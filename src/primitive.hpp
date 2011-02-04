#ifndef PRIMITIVE_HPP
#define PRIMITIVE_HPP

#include "point.hpp"
#include "ray.hpp"
#include "transform.hpp"

class Primitive;


/*
 * Having a struct that holds all of the geometry only aspects allows a clean 
 * abstraction between geometry and shading. This struct can be extended to 
 * encapsulate the differntials of the surface at the point of intersection.
 *
 * In particular this will be used primarily to represent the area close to an
 * intersection; the point, normal to the point, u v parameterization at that
 * point and differential geometry.
 *
 * This has been adapted from PBRT.
 */
typedef struct PrimitivePatch 
{
	PrimitivePatch(); 
	PrimitivePatch(const Point &P, 
                  double _u, 
                  double _v,
                  const Vector &DPDU,
                  const Vector &DPDV,
                  const Vector &DNDU,
                  const Vector &DNDV,
                  const Primitive *pr,
                  bool flipNormal);
   PrimitivePatch(const PrimitivePatch &otherPatch);
   
   // All in world coordinates.
   
   Point p;          // Point of intersection.
	Normal geomNorm;  // Normalized normal at that point on the surface
   Normal shadingNorm; // Always in the direction of the viewer and possibly bump mapped.
   
   /*
    * The u, v parameters for the primitive. All primitives that this ray-tracer
    * supports can be parametrically defined in 2 parameters (u,v). Assuming
    * f is the parametric function of the primitive p = f(u,v)
    */
   double u, v;
   
   // partial derivatives of the vectors spanning the tangent plane at point p.
   Vector dpdu, dpdv;
   // partial change in normal vector along u and v spanning vectors.
   Vector dndu, dndv;
   
   const Primitive *primitive;
   
} PrimitivePatch;


class Primitive 
{
public:
   virtual ~Primitive();
   
   virtual void prepare(const Transform &wolrdToPrimitive);
   
   virtual const BBox &objectBounds() const;
   virtual const BBox &worldBounds() const;
   
   virtual size_t intersect(const Ray &r, 
                            double *tHitNear, 
                            PrimitivePatch *pPatchNear,
                            double *tHitFar = NULL, 
                            PrimitivePatch *pPatchFar = NULL) const;
   virtual bool intersects(const Ray &r) const;
   
protected:
      
   Primitive(BBox objectBounds = BBox()) : mObjectBounds(objectBounds){};
   
   virtual size_t computeIntersectionT(const Ray &ray, 
                                       double *t0, 
                                       double *t1) const = 0;
   virtual void computeIntersectionInfo(const Point &p, 
                                        PrimitivePatch *pPatch) const = 0;
   
   size_t intersectionForQuadratic(double A, double B, double C,
                                   double minT, double maxT,
                                   double *t0, double *t1) const;
   
   size_t clampAlongZ(const Ray &ray,
                      double length,
                      size_t solutions,
                      double *t0, 
                      double *t1) const;
   
   BBox mObjectBounds;
      
   /*
    * Cached values set during prepare().
    */
   Transform mWorldToPrimitiveTrans;
   BBox mWorldBounds;
};

#endif // PRIMITIVE_HPP
