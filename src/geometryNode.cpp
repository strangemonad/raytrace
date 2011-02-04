/*
 *  geometryNode.hpp
 *  A4
 *
 *  Nodes that are responsible for representing primitives in a scene.
 *
 *  Created by Shawn Morel on 6/24/06.
 *  smorel - 20090740
 *  Copyright 2006.
 *
 */

#include <string>


#include "defs.hpp"
#include "geometryNode.hpp"
#include "material.hpp"
#include "prefs.hpp"
#include "primitive.hpp"
#include "stats.hpp"


/*
 *------------------------------------------------------------------------------
 * 
 * GeometryNode::GeometryNode --
 *     
 *     Constructor.
 *
 *------------------------------------------------------------------------------
 */

GeometryNode::GeometryNode(const std::string& name, // IN
                           Primitive* primitive,    // IN
                           Material* material)      // IN
   : SceneNode(name),
     mPrimitive(primitive),
     mMaterial(material)
{
}


/*
 *------------------------------------------------------------------------------
 * 
 * GeometryNode::GeometryNode --
 *     
 *     Copy Constructor.
 *
 *------------------------------------------------------------------------------
 */

GeometryNode::GeometryNode(const GeometryNode& otherNode) // IN
   : SceneNode(otherNode),
     mPrimitive(otherNode.mPrimitive),
     mMaterial(otherNode.mMaterial)
{
}


/*
 *------------------------------------------------------------------------------
 * 
 * GeometryNode::operator = --
 *     
 *     Assignment operator.
 *
 *------------------------------------------------------------------------------
 */

GeometryNode& 
GeometryNode::operator=(const GeometryNode& otherNode) // IN
{
   setName(otherNode.getName());
   mPrimitive = otherNode.mPrimitive;
   mMaterial = otherNode.mMaterial;
   
   return *this;
}


/*
 *------------------------------------------------------------------------------
 * 
 * GeometryNode::~GeometryNode --
 *     
 *     Destructor.
 *
 *------------------------------------------------------------------------------
 */

GeometryNode::~GeometryNode()
{
}


/*
 *------------------------------------------------------------------------------
 * 
 * GeometryNode::prepare --
 *     
 *     A scene is considered immutable while rendering. Prepare will be called
 *     immediately before rendering the scene and allow things to be cached.
 *
 *------------------------------------------------------------------------------
 */

void
GeometryNode::prepare(const Transform &wolrdToSuperNode)
{
   ASSERT(mPrimitive);
   
   mWolrdToNodeTrans = mTrans.getInverse() * wolrdToSuperNode;
   
   mPrimitive->prepare(mWolrdToNodeTrans);
   
   if (Prefs::sharedPrefs().boundsChecking) {
      // XXX node bounds might not be right
      mNodeBounds = mTrans(mPrimitive->objectBounds());
      mWorldBounds = mPrimitive->worldBounds();
   }
}


/*
 *------------------------------------------------------------------------------
 * 
 * GeometryNode::getMaterial --
 *     
 *     Accessor to get a constant ptr to the material applied to the primitive.
 *     May be null if no material to apply.
 *
 *------------------------------------------------------------------------------
 */

const Material *
GeometryNode::getMaterial() const
{
   return mMaterial;
}


/*
 *------------------------------------------------------------------------------
 * 
 * GeometryNode::getMaterial --
 *     
 *     Accessor to get a constant ref of the material applied to the primitive.
 *
 *------------------------------------------------------------------------------
 */

void 
GeometryNode::setMaterial(Material *material) // IN
{
   mMaterial = material;
}


/*
 *------------------------------------------------------------------------------
 * 
 * GeometryNode::intersect --
 *     
 *     Calculates to see if there is an intersection for the current ray given
 *     its current parametric clamping. If there is the ray is clamped.
 *
 *------------------------------------------------------------------------------
 */

size_t 
GeometryNode::intersect(const Ray &r, 
                        NodeIntersection *nIntersectionNear,
                        double *tHitFar,
                        NodeIntersection *nIntersectionFar) const
{
   ASSERT(mPrimitive);

   // Quickly check the bounding box first
   double tHit;
   if (Prefs::sharedPrefs().boundsChecking){
      if (!mWorldBounds.intersects(r)) {
         return false;
      }
   }

   
   // XXX how to handle t clamping?
   
   Stats::sharedStats().primitiveIntersections += 1;
   size_t solutions = 0;
   if (!(solutions = mPrimitive->intersect(r, 
                                           &tHit, 
                                           &nIntersectionNear->pPatch,
                                           tHitFar,
                                           &nIntersectionFar->pPatch))) {
      return 0;
   }
   nIntersectionNear->mat = mMaterial;
   
   if (nIntersectionFar) {
      nIntersectionFar->mat = mMaterial;
   }
   
   // Clamp the ray.
   r.maxt = tHit;
   
   
   return solutions;
}


/*
 *------------------------------------------------------------------------------
 * 
 * GeometryNode::intersects --
 *     
 *     Calculates to see if there is an intersection for the current ray given
 *     its current parametric clamping. If there is the ray is clamped.
 *     This is the more efficient version.
 *
 *------------------------------------------------------------------------------
 */

bool 
GeometryNode::intersects(const Ray &r, double *transmissiveness) const
{
   ASSERT(mPrimitive);
 
   // Quickly check the bounding box first
   if (Prefs::sharedPrefs().boundsChecking){
      if (!mWorldBounds.intersects(r)) {
         return false;
      }
   }
   
   // XXX how to handle t clamping?
   
   Stats::sharedStats().rapidIntersections += 1;
   
   bool doesIntersect = mPrimitive->intersects(r);
   if (transmissiveness) {
      if (doesIntersect) {
         *transmissiveness = 1 - mMaterial->getOpacityApprox();
      } else {
         *transmissiveness = 1.0;
      }
   }
   
   return doesIntersect;
}
