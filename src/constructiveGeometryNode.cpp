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

#include "constructiveGeometryNode.hpp"
#include "defs.hpp"
#include "material.hpp"
#include "prefs.hpp"
#include "primitive.hpp"
#include "stats.hpp"


/*
 *------------------------------------------------------------------------------
 * 
 * ConstructiveGeometryNode::ConstructiveGeometryNode --
 *     
 *     Constructor.
 *
 *------------------------------------------------------------------------------
 */

ConstructiveGeometryNode::ConstructiveGeometryNode(const std::string& name, 
                                                   SceneNode *a,
                                                   SceneNode *b,
                                                   CSGOp operation,
                                                   Material *material)
   : GeometryNode(name, NULL, material),
     mNodeA(a),
     mNodeB(b),
     op(operation)
{
}


/*
 *------------------------------------------------------------------------------
 * 
 * ConstructiveGeometryNode::ConstructiveGeometryNode --
 *     
 *     Copy Constructor.
 *
 *------------------------------------------------------------------------------
 */

ConstructiveGeometryNode::ConstructiveGeometryNode(const ConstructiveGeometryNode& otherNode) // IN
   : GeometryNode(otherNode),
     mNodeA(otherNode.mNodeA),
     mNodeB(otherNode.mNodeB),
     op(otherNode.op)
{
}


/*
 *------------------------------------------------------------------------------
 * 
 * ConstructiveGeometryNode::operator= --
 *     
 *     Assignment operator.
 *
 *------------------------------------------------------------------------------
 */

ConstructiveGeometryNode & 
ConstructiveGeometryNode::operator=(const ConstructiveGeometryNode& otherNode) // IN
{
   setName(otherNode.getName());
   mMaterial = otherNode.mMaterial;
   mNodeA = otherNode.mNodeA;
   mNodeB = otherNode.mNodeB;
   op = otherNode.op;
   
   return *this;
}


/*
 *------------------------------------------------------------------------------
 * 
 * ConstructiveGeometryNode::~ConstructiveGeometryNode --
 *     
 *     Destructor.
 *
 *------------------------------------------------------------------------------
 */

ConstructiveGeometryNode::~ConstructiveGeometryNode()
{
}


/*
 *------------------------------------------------------------------------------
 * 
 * ConstructiveGeometryNode::prepare --
 *     
 *     A scene is considered immutable while rendering. Prepare will be called
 *     immediately before rendering the scene and allow things to be cached.
 *
 *------------------------------------------------------------------------------
 */

void
ConstructiveGeometryNode::prepare(const Transform &wolrdToSuperNode)
{
   mWolrdToNodeTrans = mTrans.getInverse() * wolrdToSuperNode;
   
   mNodeA->prepare(mWolrdToNodeTrans);
   mNodeB->prepare(mWolrdToNodeTrans);
   
   if (Prefs::sharedPrefs().boundsChecking) {      
      switch (op) {
      case UNION:
         // XXX node bounds might not be right
         mNodeBounds = boxUnion(mTrans(mNodeA->nodeBounds()), 
                                mTrans(mNodeB->nodeBounds()));
         mWorldBounds = boxUnion(mNodeA->worldBounds(), 
                                 mNodeB->worldBounds());
         break;
      case INTERSECTION:
         // XXX node bounds might not be right
         mNodeBounds = boxIntersection(mTrans(mNodeA->nodeBounds()), 
                                       mTrans(mNodeB->nodeBounds()));
         mWorldBounds = boxIntersection(mNodeA->worldBounds(), 
                                        mNodeB->worldBounds());
         break;
      case DIFFERENCE:
         // XXX node bounds might not be right
         mNodeBounds = mTrans(mNodeA->nodeBounds());
         mWorldBounds = mNodeA->worldBounds();
         break;
      default:
         ASSERT_NOT_REACHED;
         break;
      }

   }
}


/*
 *------------------------------------------------------------------------------
 * 
 * ConstructiveGeometryNode::intersect --
 *     
 *     Calculates to see if there is an intersection for the current ray given
 *     its current parametric clamping. If there is the ray is clamped.
 *
 *------------------------------------------------------------------------------
 */

size_t 
ConstructiveGeometryNode::intersect(const Ray &r, 
                                    NodeIntersection *nIntersectionNear,
                                    double *tHitFar,
                                    NodeIntersection *nIntersectionFar) const
{
   // XXX need copies of rays since they will do clamping... bad bad
   
   // Quickly check the bounding box first
   if (Prefs::sharedPrefs().boundsChecking){
      if (!mWorldBounds.intersects(r)) {
         return false;
      }
   }
#if 0
   // XXX how to handle t clamping of the passed in ray? 
   Ray rA = r;
   Ray rB = r;
   NodeIntersection iNearA;
   NodeIntersection iNearB;
   
   /*
    * This implementation only works for solids of at most degree 2. These
    * solids will either have 0, 1, or 2 intersections. This means that each
    * case (union, intersection, and difference) must properly handle 0 - 4
    * intersections and return the approriate number of intersections
    * between 0 - 2. This will allow CGNode to be completey opaque and used in
    * other CGNodes.
    */
   
   double tHitFarA, tHitFarB;
   NodeIntersection iFarA, iFarB;
   size_t solutionsA = 0;
   size_t solutionsB = 0;
   
   intersectsA = mNodeA->intersect(rA, &iA, &tHitFarA);
   intersectsB = mNodeB->intersect(rB, &iB, &tHitFarB, &iFarB);
   
   if (!(intersectsA || intersectsB)) { 
      return false
   }

   // XXX need to determine the far hit as well to compose multiple csgs.
   switch (op) {
      case UNION:
      {
         /*
          * The resulting near intersection is the closest of the near 
          * intersections of A and B and the far is the furthest of the 2.
          */
         if (intersectsA && intersectsB) {
            if (rA.maxt <= rB.maxt) {
               r.maxt = rA.maxt;
               *nIntersectionNear =  NodeIntersection(iA);
               if (mMaterial) {
                  nIntersectionNear->mat = mMaterial;
               }
            } else {
               r.maxt = rB.maxt;
               *nIntersectionNear =  NodeIntersection(iB);
               if (mMaterial) {
                  nIntersectionNear->mat = mMaterial;
               }
            }
            return true;
            
         } else if (intersectsA) {
            r.maxt = rA.maxt;
            *nIntersectionNear =  NodeIntersection(iA);
            if (mMaterial) {
               nIntersectionNear->mat = mMaterial;
            }
            return true;
            
         } else if (intersectsB) {
            r.maxt = rA.maxt;
            *nIntersectionNear =  NodeIntersection(iB);
            if (mMaterial) {
               nIntersectionNear->mat = mMaterial;
            }
            return true;
            
         } else {
            return false;
         }
         break;
      }
         
      case INTERSECTION:
      {
         double tHitFarA, tHitFarB;
         
         intersectsA = mNodeA->intersect(rA, &iA, &tHitFarA);
         intersectsB = mNodeB->intersect(rB, &iB, &tHitFarB); 
         
         /*
          * Choose the intersection that corresponds to the largest t value
          * of both near intersections.
          */
         if (intersectsA && intersectsB) {
            if (!finite(tHitFarA)) {
               // There is no far intersection 
            }
            if (rA.maxt <= rB.maxt) {
               r.maxt = rB.maxt;
               *nIntersectionNear =  NodeIntersection(iB);
               if (mMaterial) {
                  nIntersectionNear->mat = mMaterial;
               }
            } else {
               r.maxt = rA.maxt;
               *nIntersectionNear =  NodeIntersection(iA);
               if (mMaterial) {
                  nIntersectionNear->mat = mMaterial;
               }
            }
            return true;
         } else {
            return false;
         }
         break;
      }
         
      case DIFFERENCE:
      { 
         if (intersectsA && !intersectsB) {
            r.maxt = rA.maxt;
            *nIntersectionNear =  NodeIntersection(iA);
            if (mMaterial) {
               nIntersectionNear->mat = mMaterial;
            }
            return true;
         } else if (intersectsA && intersectsB) {
            if (rA.maxt < rB.maxt || tHitFarB < rA.maxt) {
               r.maxt = rA.maxt;
               *nIntersectionNear =  NodeIntersection(iA);
               if (mMaterial) {
                  nIntersectionNear->mat = mMaterial;
               }
               return true;
            } else if (tHitFarB < tHitFarA) {
               r.maxt = tHitFarB;
               *nIntersectionNear =  NodeIntersection(iFarB);
               nIntersectionNear->pPatch.geomNorm = -nIntersectionNear->pPatch.geomNorm;
               // the shading norm is already properly calculated.
               if (mMaterial) {
                  nIntersectionNear->mat = mMaterial;
               }
               return true;
            } else {
               return false;
            }
         } else {
            return false;
         }
         break;
      }
      default:
         ASSERT_NOT_REACHED;
         return false;
         break;
   }
   
#endif
   return 0;
}


/*
 *------------------------------------------------------------------------------
 * 
 * ConstructiveGeometryNode::intersects --
 *     
 *     Calculates to see if there is an intersection for the current ray given
 *     its current parametric clamping. If there is the ray is clamped.
 *     This is the more efficient version.
 *
 *------------------------------------------------------------------------------
 */

bool 
ConstructiveGeometryNode::intersects(const Ray &r, double *transmissiveness) const
{
   // Quickly check the bounding box first
   double tHitNear, tHitFar;
   if (Prefs::sharedPrefs().boundsChecking){
      if (!mWorldBounds.intersects(r, &tHitNear, &tHitFar)) {
         return false;
      }
   }
   
   // XXX how to handle t clamping?
   
   Stats::sharedStats().rapidIntersections += 1;
   // XXX need to handle this.
   *transmissiveness = 1.0;
   return false;
}
