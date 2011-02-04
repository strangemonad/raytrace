

#include <cmath>

#include "algebra.hpp"
#include "defs.hpp"
#include "paraboloid.hpp"
#include "point.hpp"


Paraboloid::Paraboloid(double r,
           double l)
   : Primitive(BBox(Point(-r, -r, 0), Point(r, r, l))),
     mRadius(r),
     mLength(l)
{
}


Paraboloid::~Paraboloid()
{
}


size_t 
Paraboloid::computeIntersectionT(const Ray &ray, 
                                 double *t0, 
                                 double *t1) const
{
   ASSERT(t0);
   ASSERT(t1);
   
   /*
    * A, B, and C determined by substituting the parametric eqn for the ray
    * into the implicit eqn for a this paraboloid.
    */
   double k = mLength / (mRadius*mRadius);
	double A =   k*(ray.d.x * ray.d.x + 
                   ray.d.y * ray.d.y);
	double B = 2*k*(ray.d.x * ray.o.x + 
                   ray.d.y * ray.o.y) -
              ray.d.z;
	double C = k*(ray.o.x * ray.o.x + 
                 ray.o.y * ray.o.y) -
              ray.o.z;
   
   size_t solutions = 0;
   if (! (solutions = intersectionForQuadratic(A, B, C, 
                                               ray.mint, ray.maxt, 
                                               t0, t1))) {
      return false;
   }
   
   return clampAlongZ(ray, mLength, solutions, t0, t1);
}


void 
Paraboloid::computeIntersectionInfo(const Point &p, 
                                    PrimitivePatch *pPatch) const
{
   double phi = atan2(p.y, p.x);
   if (lessThanZero(phi)) {
      phi += (2.0*M_PI);
   }
   
   double u = phi / (2*M_PI);
	double v = (p.z) / (mLength);
   
	// Compute parabaloid dpdu and dpdv
	Vector dpdu(-2*M_PI * p.y, 2*M_PI * p.x, 0.);
	Vector dpdv = (mLength) * Vector(p.x / (2.0 * p.z), 
                                        p.y / (2.0 * p.z), 
                                        1.0);
   
	// Compute parabaloid dndu and dndv
	Vector d2Pduu = -2*M_PI * 2*M_PI * Vector(p.x, p.y, 0);
	Vector d2Pduv = (mLength) * 2*M_PI * Vector(-p.y / (2.0 * p.z),
                                                   p.x / (2.0 * p.z),
                                                   0);
	Vector d2Pdvv = -(mLength) * (mLength) * Vector(p.x/(4.f*p.z*p.z),
                                                           p.y/(4.f*p.z*p.z),
                                                           0);
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
