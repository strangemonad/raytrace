

#include <cmath>

#include "defs.hpp"
#include "algebra.hpp"
#include "polyroots.hpp"
#include "cube.hpp"

// XXX currently disregard side length should maybe fix this
Cube::Cube(double s)
    : Primitive(BBox(Point(0, 0, 0), Point(s, s, s))),
      mSideLength(s)
{
}


Cube::~Cube()
{
}


/*
 * The cube is our only non-parametric surface. We can't rely on Primitive's
 * implementation to just call computeInterSectionInfo(pointIn, *patchOut)
 * since we need more info than just the point (what cube face it was on).
 */
size_t 
Cube::intersect(const Ray &r, 
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
   
   if (tHitFar || pPatchFar) {
      WARN("far intersection not implemented in cube yet... CGNodes will cause problems");
   }
   
   /*
    * Now that the coords have been transformed we are dealing with a unit
    * cube with one corner at (0, 0, 0) and the opposite corner at (1, 1, 1).
    * Test each plane that makes up the unit cube and then clamp to the unit 
    * square.
    */
   
   double tempT = INFINITY;
   *tHitNear = INFINITY;
   Point tempIntersection;
   bool doesIntersect = false;
   //*pPatchNear = PrimitivePatch();
   
   // Test intersection with each plane
   // XXX could optimize to break out after 2 hits.
   for (int axis = 0; axis < 3; axis +=1) {
      
      for (int plane = 0; plane < 2; plane += 1) {
         
         tempT = (plane - ray.o[axis]) / ray.d[axis];
         tempIntersection = ray(tempT);
         
         int orthoU;
         int orthoV;
         computeOrthogonal(axis, &orthoU, &orthoV);
         
         if ((tempIntersection[orthoU] > 0.0 && tempIntersection[orthoU] < 1.0 && 
              tempIntersection[orthoV] > 0.0 && tempIntersection[orthoV] < 1.0) &&
             (tempT > ray.mint && tempT < ray.maxt) && 
             tempT < *tHitNear) {
            
            *tHitNear = tempT;
            Vector dpdu = Vector(0,0,0);
            dpdu[orthoU] = 1.0 - (2 * plane);
            double u = (plane == 0) ? tempIntersection[orthoU] : 1 - tempIntersection[orthoU];
            Vector dpdv = Vector(0,0,0);
            dpdv[orthoV] = 1;
            
            /*
             * There is no need to compute the partial derivatives dn/du and 
             * dn/dv since the face of the cube is flat it will be the 0 vector.
             */
            
            // Initialize the PrimitivePatch
            Transform primitiveToWorld = mWorldToPrimitiveTrans.getInverse();
            *pPatchNear = PrimitivePatch(primitiveToWorld(tempIntersection),
                                         u,
                                         tempIntersection[orthoV], // v
                                         primitiveToWorld(dpdu),
                                         primitiveToWorld(dpdv),
                                         Vector(0,0,0),
                                         Vector(0,0,0),
                                         this, false);
            if (lessThanZero(dot(-r.d, pPatchNear->shadingNorm))) {
               // We've found a valid intersection so make sure it is visible.
               pPatchNear->shadingNorm = - pPatchNear->shadingNorm;
            }
            
            doesIntersect = true;
         }
      }
   } 
   
	return (size_t)doesIntersect;
}


size_t 
Cube::computeIntersectionT(const Ray &ray, 
                           double *t0, 
                           double *t1) const
{
   *t0 = ray.mint;
   *t1 = ray.maxt;
   
   Point pMin = Point(0,0,0);
   //Point pMax = Point(mSideLength, mSideLength, mSideLength);
   Point pMax = Point(1, 1, 1);
   
	for (int i = 0; i < 3; ++i) {
		// Update interval for _i_th bounding box slab
		double invRayDir = 1.0 / ray.d[i];
		double tNear = (pMin[i] - ray.o[i]) * invRayDir;
		double tFar  = (pMax[i] - ray.o[i]) * invRayDir;
		if (tNear > tFar) std::swap(tNear, tFar);
		*t0 = max(tNear, *t0);
		*t1 = min(tFar, *t1);
		if (*t0 > *t1) 
      {
         return 0;
      }
	}
   
   return 1;
}


void 
Cube::computeIntersectionInfo(const Point &p, 
                              PrimitivePatch *pPatch) const
{
   /*
    * This is not a parametric surface it overrides intersect() in Primitive
    * and fills the intersection info there.
    */
   WARN("invalid call to Cube::computeIntersectionInfo");
   WARN(p);
   WARN(pPatch);
   ASSERT_NOT_REACHED;
}


void 
Cube::computeOrthogonal(int axis, int *orthoU, int *orthoV) const
{
   switch (axis) {
   case 0: // x
      *orthoU = 1;
      *orthoV = 2;
      break;
      
   case 1: // y
      *orthoU = 0;
      *orthoV = 2;
      break;
      
   case 2: // z
      *orthoU = 0;
      *orthoV = 1;
      
      break;
   default:
      break;
   }
}


#if 0 
// XXX work out far intersection
if ((tempIntersection[ortho1] > 0.0 && tempIntersection[ortho1] < 1.0 && 
     tempIntersection[ortho2] > 0.0 && tempIntersection[ortho2] < 1.0) &&
    (tempT > ray.mint && tempT < ray.maxt)) {
   
   if (tempT < *tHitNear) {
      if (tHitFar) {
         *tHitFar = *tHitNear;
         if (pPatchFar) {
            *pPatchFar = PrimitivePatch(*pPatchNear);
            pPatchFar->nn = -pPatchFar->nn;
         }
      }
      
      *tHitNear = tempT;
      
      pPatchNear->p = mWorldToPrimitiveTrans.getInverse()(tempIntersection);
      pPatchNear->nn = Normal();
      pPatchNear->nn[axis] = (2 * plane) - 1.0;
      pPatchNear->nn = normalize(mWorldToPrimitiveTrans.getInverse()(pPatchNear->nn));
      pPatchNear->primitive = this;
   } else if (tHitFar && tempT < *tHitFar) {
      *tHitFar = tempT;
      if (pPatchFar) {
         
         pPatchFar->p = mWorldToPrimitiveTrans.getInverse()(tempIntersection);
         pPatchFar->nn = Normal();
         pPatchFar->nn[axis] = 1.0 - (2 * plane);
         pPatchFar->nn = normalize(mWorldToPrimitiveTrans.getInverse()(pPatchFar->nn));
         pPatchFar->primitive = this;
      }
   }
}
#endif