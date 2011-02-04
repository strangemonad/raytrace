/*
 *  sceneNode.cpp
 *  A4
 *
 *  Base class for nodes comprising a hierarchical scene description.
 *
 *  Created by Shawn Morel on 6/24/06.
 *  smorel - 20090740
 *  Copyright 2006.
 *
 */


#include "bBox.hpp"
#include "defs.hpp"
#include "prefs.hpp"
#include "sceneNode.hpp"


NodeIntersection::NodeIntersection(const NodeIntersection &otherNode)
: pPatch(otherNode.pPatch),
mat(otherNode.mat)
{
}


/*
 *------------------------------------------------------------------------------
 * 
 * SceneNode::SceneNode --
 *     
 *     Constructor.
 *
 *------------------------------------------------------------------------------
 */

SceneNode::SceneNode(const std::string& name) // IN
   : mName(name)
{
}


/*
 *------------------------------------------------------------------------------
 * 
 * SceneNode::SceneNode --
 *     
 *     Copy Constructor.
 *
 *------------------------------------------------------------------------------
 */

SceneNode::SceneNode(const SceneNode& otherNode) // IN
   : mName(otherNode.mName)
{
}


/*
 *------------------------------------------------------------------------------
 * 
 * SceneNode::operator = --
 *     
 *     Assignment operator.
 *
 *------------------------------------------------------------------------------
 */

SceneNode& 
SceneNode::operator=(const SceneNode& otherNode) // IN
{
   mName = otherNode.getName();
   
   return *this;
}


/*
 *------------------------------------------------------------------------------
 * 
 * SceneNode::~SceneNode --
 *     
 *     Destructor.
 *
 *------------------------------------------------------------------------------
 */

SceneNode::~SceneNode()
{
}


/*
 *------------------------------------------------------------------------------
 * 
 * SceneNode::getName --
 *     
 *     Accessor for the name property of the node.
 *
 *------------------------------------------------------------------------------
 */

const std::string& 
SceneNode::getName() const
{
   return mName;
}


/*
 *------------------------------------------------------------------------------
 * 
 * SceneNode::setName --
 *     
 *     Mutator for the name property of the node.
 *
 *------------------------------------------------------------------------------
 */

void 
SceneNode::setName(const std::string& newName) // IN
{
   mName = newName;
}


/*
 *------------------------------------------------------------------------------
 * 
 * SceneNode::prepare --
 *     
 *     A scene is considered immutable while rendering. Prepare will be called
 *     immediately before rendering the scene and allow things to be cached.
 *
 *------------------------------------------------------------------------------
 */

void
SceneNode::prepare(const Transform &wolrdToSuperNode)
{   
   mWolrdToNodeTrans = mTrans.getInverse() * wolrdToSuperNode;
   
   /*
    * Preparetion of a hierarchical scene must be done as a depth first 
    * (post order) traversal.
    */
   
   for(ChildList::const_iterator childIter = mChildren.begin();
       childIter != mChildren.end();
       childIter++) {
      
      (*childIter)->prepare(mWolrdToNodeTrans);
      
      if (Prefs::sharedPrefs().boundsChecking) {
         // XXX node bounds might not be right
         mNodeBounds = boxUnion(mNodeBounds, 
                                mTrans((*childIter)->nodeBounds()));
         mWorldBounds = boxUnion(mWorldBounds, 
                                 (*childIter)->worldBounds());
      }
   }
}


/*
 *------------------------------------------------------------------------------
 * 
 * SceneNode::nodeBounds --
 *     
 *     A bounding box for this node in this node's coordinate system.
 *
 *------------------------------------------------------------------------------
 */

const BBox &
SceneNode::nodeBounds() const
{
   ASSERT(Prefs::sharedPrefs().boundsChecking);
   return mNodeBounds;
}


/*
 *------------------------------------------------------------------------------
 * 
 * SceneNode::worldBounds --
 *     
 *     A bounding box for this node in world coords
 *
 *------------------------------------------------------------------------------
 */

const BBox &
SceneNode::worldBounds() const
{
   ASSERT(Prefs::sharedPrefs().boundsChecking);
   return mWorldBounds;
}


/*
 *------------------------------------------------------------------------------
 * 
 * SceneNode::getTransform --
 *     
 *     Accessor for the current transformation matrix being applied to this 
 *     node.
 *
 *------------------------------------------------------------------------------
 */

const Transform & 
SceneNode::getTransform() const 
{ 
   return mTrans; 
}


/*
 *------------------------------------------------------------------------------
 * 
 * SceneNode::setTransform --
 *     
 *     Set the current transofrmation. Overrides the current transform and
 *     implicitly sets the inverse.
 *
 *------------------------------------------------------------------------------
 */

void 
SceneNode::setTransform(const Transform &t) // IN
{
   mTrans = t;
}


/*
 *------------------------------------------------------------------------------
 * 
 * SceneNode::addChild --
 *     
 *     Add a child node to this node.
 *
 *------------------------------------------------------------------------------
 */

void 
SceneNode::addChild(SceneNode* child) // IN
{
   mChildren.push_back(child);
}


/*
 *------------------------------------------------------------------------------
 * 
 * SceneNode::removeChild --
 *     
 *     Remove the specified child from the list of children if it is in there.
 *
 *------------------------------------------------------------------------------
 */

void 
SceneNode::removeChild(SceneNode* child) // IN
{
   mChildren.remove(child);
}


/*
 *------------------------------------------------------------------------------
 * 
 * SceneNode::rotate --
 *     
 *     Compose a rotation onto whatever transform is currently affecting this
 *     node.
 *
 *------------------------------------------------------------------------------
 */

void 
SceneNode::rotate(char axis,    // IN
                  double angle) // IN
{
   Transform r;
   switch (axis) {
      case 'x':
         r = RotateX(angle);
         break;
      case 'y':
         r= RotateY(angle);
         break;
      case 'z':
         r= RotateZ(angle);
         break;
      default:
         break;
   }
   
   setTransform(r * mTrans);
}


/*
 *------------------------------------------------------------------------------
 * 
 * SceneNode::scale --
 *     
 *     Compose a scale onto whatever transform is currently affecting this
 *     node.
 *
 *------------------------------------------------------------------------------
 */

void 
SceneNode::scale(const Vector &amount) // IN
{
   setTransform(Scale(amount[0], amount[1], amount[2]) * mTrans);
}


/*
 *------------------------------------------------------------------------------
 * 
 * SceneNode::SceneNode --
 *     
 *     Compose a translation onto whatever transform is currently affecting
 *     this node.
 *
 *------------------------------------------------------------------------------
 */

void 
SceneNode::translate(const Vector &amount) // IN
{
   setTransform(Translate(amount) * mTrans);
}


/*
 *------------------------------------------------------------------------------
 * 
 * SceneNode::isJoint --
 *     
 *     Check if the current node is a joint node. Not as important for the ray 
 *     tracer since scenes can't be interactively manipulated. The sceneNode
 *     implementation always returns false.
 *
 *------------------------------------------------------------------------------
 */

bool 
SceneNode::isJoint() const
{
   return false;
}


/*
 *------------------------------------------------------------------------------
 * 
 * SceneNode::intersect --
 *     
 *     Calculates the intersection of ray with all the children and determines
 *     if the closest intersection out of all children. The values in
 *     nIntersection are meaningless if false is returned.
 *
 *------------------------------------------------------------------------------
 */

size_t 
SceneNode::intersect(const Ray &r, 
                     NodeIntersection *nIntersectionNear,
                     double *tHitFar,
                     NodeIntersection *nIntersectionFar) const
{
   // Quickly check the bounding box first
   if (Prefs::sharedPrefs().boundsChecking){
      if (!mWorldBounds.intersects(r)) {
         return false;
      }
   }

   // XXX how to handle t clamping?
   size_t solutions = 0;
   //Transform curTrans = mTrans.getInverse() * mWolrdToNodeTrans;   
   for(ChildList::const_iterator childIter = mChildren.begin();
       childIter != mChildren.end();
       childIter++) {
      /*
       * Each primitive will return no intersections if the ray clamped to the
       * t values doesn't intersect it. Iterating over all the nodes will
       * in fact return the closest intersection.
       */
      size_t curSolns = (*childIter)->intersect(r, nIntersectionNear, 
                                                tHitFar, nIntersectionFar);
      if (curSolns) {
         solutions = curSolns;
      }
   }
   
   return solutions;
}


/*
 *------------------------------------------------------------------------------
 * 
 * SceneNode::intersects --
 *     
 *     Calculates the intersection of ray with all the children and determines
 *     if the closest intersection out of all children. This is more efficient
 *     than the other intersect method because some calculations are left out.
 *
 *------------------------------------------------------------------------------
 */

bool 
SceneNode::intersects(const Ray &r,
                       double *transmissiveness) const
{
   // xxx should factor things out and use a function pointer or something. 
   
   // Quickly check the bounding box first
   double tHitNear, tHitFar;
   if (Prefs::sharedPrefs().boundsChecking){
      if (!mWorldBounds.intersects(r, &tHitNear, &tHitFar)) {
         return false;
      }
   }
   
   // XXX how to handle t clamping?

   //Transform curTrans = mTrans.getInverse() * mWolrdToNodeTrans;
   
   // Depth first search of the tree. This can be much improved!!
   bool doesIntersect = false;
   double childOpacity;
   
   if (transmissiveness){
      *transmissiveness = 1.0;
   }
   for(ChildList::const_iterator childIter = mChildren.begin();
       childIter != mChildren.end();
       childIter++) {
      
      if (transmissiveness) {
         doesIntersect = (*childIter)->intersects(r, &childOpacity);
         
         // Accumulate resulting light transmitted
         *transmissiveness = *transmissiveness * childOpacity;
      } else if ((*childIter)->intersects(r)) {
         return true;
      }
   }
   
   return doesIntersect;
}

