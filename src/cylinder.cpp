

#include <cmath>

#include "algebra.hpp"
#include "defs.hpp"
#include "cylinder.hpp"
#include "point.hpp"


Cylinder::Cylinder(double r,
                   double l)
   : Primitive(BBox(Point(-r, -r, 0), Point(r, r, l))),
     mRadius(r),
     mLength(l)
{
}


Cylinder::~Cylinder()
{
}


size_t 
Cylinder::computeIntersectionT(const Ray &ray, 
                               double *t0, 
                               double *t1) const
{
   ASSERT(t0);
   ASSERT(t1);
   
   /*
    * A, B, and C determined by substituting the parametric eqn for the ray
    * into the implicit eqn for a this cylinder.
    */
   double A = ray.d.x * ray.d.x + 
              ray.d.y * ray.d.y;
	double B = 2 * (ray.d.x * ray.o.x + 
                   ray.d.y * ray.o.y);
	double C = ray.o.x * ray.o.x + 
              ray.o.y * ray.o.y -
              mRadius * mRadius;
   size_t solutions = 0;
   if (! (solutions = intersectionForQuadratic(A, B, C, 
                                               ray.mint, ray.maxt, 
                                               t0, t1))) {
      return 0;
   }
   
   return clampAlongZ(ray, mLength, solutions, t0, t1);
}


void 
Cylinder::computeIntersectionInfo(const Point &p, 
                                  PrimitivePatch *pPatch) const
{
   double phi = atan2(p.y, p.x);
	if (lessThanZero(phi)) {
      phi += (2.0*M_PI);
   }
   
   // Find parametric representation of cylinder hit
   double u = phi / (2*M_PI);
   double v = (p.z) / (mLength);
   
	// Compute cylinder dpdu and dpdv
	Vector dpdu(-2*M_PI * p.y, 2*M_PI * p.x, 0);
	Vector dpdv(0, 0, mLength);
   
   // Compute sphere dndu and dndv using Weingarten equations.
	Vector d2Pduu = -2*M_PI * 2*M_PI * Vector(p.x, p.y, 0);
	Vector d2Pduv(0, 0, 0);
   Vector d2Pdvv(0, 0, 0);
	// Compute coefficients for fundamental forms
	double E = dot(dpdu, dpdu);
	double F = dot(dpdu, dpdv);
	double G = dot(dpdv, dpdv);
	Vector N = normalize(cross(dpdu, dpdv));
	double e = dot(N, d2Pduu);
	double f = dot(N, d2Pduv);
	double g = dot(N, d2Pdvv);
	// Compute dndu and dndv from fundamental form coefficients
	double invEGF2 = 1.0 / (E*G - F*F);
   Vector dndu = (f*F - e*G) * invEGF2 * dpdu + (e*F - f*E) * invEGF2 * dpdv;
	Vector dndv = (g*F - f*G) * invEGF2 * dpdu + (f*F - g*E) * invEGF2 * dpdv;
	
   // Initialize the PrimitivePatch
   Transform primitiveToWorld = mWorldToPrimitiveTrans.getInverse();
   *pPatch = PrimitivePatch(primitiveToWorld(p),
                            u, v,
                            primitiveToWorld(dpdu),
                            primitiveToWorld(dpdv),
                            primitiveToWorld(dndu),
                            primitiveToWorld(dndv),
                            this, false);
}
