
#include <math.h>

#include "algebra.hpp"
#include "bBox.hpp"
#include "defs.hpp"
#include "prefs.hpp"
#include "primitive.hpp"
#include "polyroots.hpp"
#include "transform.hpp"


PrimitivePatch::PrimitivePatch()
: u(0.0), 
v(0.0), primitive(NULL)
{
}


PrimitivePatch::PrimitivePatch(const Point &P, 
                               double _u, 
                               double _v,
                               const Vector &DPDU,
                               const Vector &DPDV,
                               const Vector &DNDU,
                               const Vector &DNDV,
                               const Primitive *pr,
                               bool flipNormal)
: p(P),
u(_u),
v(_v),
dpdu(DPDU), 
dpdv(DPDV), 
dndu(DNDU),
dndv(DNDV),
primitive(pr)
{
   geomNorm = normalize(Normal(cross(dpdu, dpdv)));
   if (flipNormal) {
      geomNorm= -geomNorm;
   }
   shadingNorm = geomNorm;
}


PrimitivePatch::PrimitivePatch(const PrimitivePatch &otherPatch)
: p(otherPatch.p),
geomNorm(otherPatch.geomNorm),
shadingNorm(otherPatch.shadingNorm),
u(otherPatch.u),
v(otherPatch.v),
dpdu(otherPatch.dpdu),
dpdv(otherPatch.dpdv),
dndu(otherPatch.dndu),
dndv(otherPatch.dndv),
primitive(otherPatch.primitive)
{
}



Primitive::~Primitive()
{
}

void 
Primitive::prepare(const Transform &wolrdToPrimitive)
{
   ASSERT(!mObjectBounds.isDegenerate());
   
   mWorldToPrimitiveTrans = wolrdToPrimitive;
   
   // XXX consider allowing subclasses to calculate a tighter world bound.
   mWorldBounds = mWorldToPrimitiveTrans.getInverse()(mObjectBounds);
}


const BBox &
Primitive::objectBounds() const
{
   ASSERT(Prefs::sharedPrefs().boundsChecking);
   return mObjectBounds;
}


const BBox &
Primitive::worldBounds() const
{
   ASSERT(Prefs::sharedPrefs().boundsChecking);
   return mWorldBounds;
}


size_t 
Primitive::intersect(const Ray &r, 
                     double *tHitNear, 
                     PrimitivePatch *pPatchNear,
                     double *tHitFar, 
                     PrimitivePatch *pPatchFar) const
{
   ASSERT(tHitNear);
   ASSERT(pPatchNear);
   
   /*
    * Transform the ray to object space but don't normalize. This way we can
    * use a global t value for tHit
    */
	Ray ray;
	mWorldToPrimitiveTrans(r, &ray);
   
   double t0, t1;
   size_t solutions;
   if (! (solutions = computeIntersectionT(ray, &t0, &t1))){
      return 0;
   }
   
   ASSERT(!(t0 > ray.maxt || t1 < ray.mint));
   ASSERT(!(t0 < ray.mint && t1 > ray.maxt));
   
   
   /* 
    * We have at least 1 solution so fill it in.
    */
   *tHitNear = t0;
   computeIntersectionInfo(ray(*tHitNear), pPatchNear);
   
   if (lessThanZero(dot(-r.d, pPatchNear->shadingNorm))) {
      // We've found a valid intersection so make sure it is visible.
      pPatchNear->shadingNorm = - pPatchNear->shadingNorm;
   }
   
   if (solutions == 2) {
      if (tHitFar) {
         *tHitFar = t1;
         if (pPatchFar) {
            // XXX should use isnan() but couldn't link for some reason.
            computeIntersectionInfo(ray(*tHitFar), pPatchFar);
            
            if (lessThanZero(dot(-r.d, pPatchFar->shadingNorm))) {
               // We've found a valid intersection so make sure it is visible.
               pPatchFar->shadingNorm = - pPatchFar->shadingNorm;
            }
         }
      }
   } else {
      if (tHitFar) {
         *tHitFar = INFINITY;
      }
   }
      
   return solutions;
}


bool 
Primitive::intersects(const Ray &r) const
{
   if (Prefs::sharedPrefs().boundsChecking){
      if (!mWorldBounds.intersects(r)) {
         return false;
      }
   }
   
   /*
    * Transform the ray to object space but don't normalize. This way we can
    * use a global t value for tHit
    */
	Ray ray;
	mWorldToPrimitiveTrans(r, &ray);
   
   double t0, t1;   
	if (!computeIntersectionT(ray, &t0, &t1)){
      return false;
   }
   ASSERT(!(t0 > ray.maxt || t1 < ray.mint));
   ASSERT(!(t0 < ray.mint && t1 > ray.maxt));
   
   return true;
}

// number of solutions.
size_t 
Primitive::intersectionForQuadratic(double A, double B, double C,
                                    double minT, double maxT,
                                    double *t0, double *t1) const
{
   ASSERT(t0);
   ASSERT(t1);
   
   double t[2];
   t[0] = INFINITY;
   t[1] = INFINITY;
   size_t solutions = 0;
	if (! (solutions = quadraticRoots(A, B, C, t))) {
      return 0;
   }
   if (t[0] > t[1]) {
      std::swap(t[0], t[1]);
   }
   
   if (t[0] > maxT || t[1] < minT) { // Degenerate ray.
      return 0;
   }
   
   
   if (t[0] < minT) {
      // Although we intersected at t[0] it is out of range. Check t[1]
      
      if (t[1] > maxT) {
         // t[1] also out of range.
         return 0;
      }
      
      ASSERT(solutions == 2);
      solutions -= 1;
      t[0] = t[1];
      t[1] = INFINITY;
   } else if ((solutions == 2) && (t[1] > maxT)){
      // t[0] is in range and t[1] is a soln but wasn't in range so discard it.
      t[1] = INFINITY;
      solutions -= 1;
   }
   
   *t0 = t[0];
   *t1 = t[1];
   
   return solutions;
}

size_t 
Primitive::clampAlongZ(const Ray &ray,
                       double length,
                       size_t solutions,
                       double *t0, 
                       double *t1) const
{
   ASSERT(t0);
   ASSERT(t1);
   
   /* 
    * We now have 1 or 2 solutions along an infinite quadratic (along the 
    * z-axis). Clamp the result.
    */
	Point p = ray(*t0);
	if (p.z < 0.0 || p.z > length) {
      // p misses - check if we had a second intersection
      
      if (solutions == 2) {
         p = ray(*t1);
         if (p.z < 0.0 || p.z > length) {
            return 0;
         }
         
         *t0 = *t1;
         *t1 = INFINITY;
         
         // XXX this may be causing normal vector problems.
      } else {
         *t0 = INFINITY;
         *t1 = INFINITY;
         return 0;
      }
   } else {
      /*
       * We got the first intersection but for CSG we also need to check if the
       * second intersection was right since we need near and far intersections.
       */
      if (solutions == 2) {
         p = ray(*t1);
         if (p.z < 0.0 || p.z > length) {
            *t1 = INFINITY;
         }
      } else {
         *t1 = INFINITY;
      }
   }
   
   return solutions;
}

