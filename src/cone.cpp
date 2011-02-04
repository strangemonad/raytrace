

#include <cmath>

#include "algebra.hpp"
#include "cone.hpp"
#include "defs.hpp"
#include "point.hpp"


Cone::Cone(double r,
           double l)
   : Primitive(BBox(Point(-r, -r, 0), Point(r, r, l))),
     mRadius(r),
     mLength(l)
{
}


Cone::~Cone()
{
}


size_t 
Cone::computeIntersectionT(const Ray &ray, 
                           double *t0, 
                           double *t1) const
{
   ASSERT(t0);
   ASSERT(t1);
   
   /*
    * A, B, and C determined by substituting the parametric eqn for the ray
    * into the implicit eqn for a this cone.
    */
   double k = mRadius / mLength;
	k = k*k;
	double A = ray.d.x * ray.d.x + 
              ray.d.y * ray.d.y -
		        k * ray.d.z * ray.d.z;
	double B = 2 * (ray.d.x * ray.o.x + 
                   ray.d.y * ray.o.y -
                   k * ray.d.z * (ray.o.z - mLength) );
	double C = ray.o.x * ray.o.x + 
              ray.o.y * ray.o.y -
		        k * (ray.o.z - mLength) * (ray.o.z-mLength);
   
   size_t solutions = 0;
   if (! (solutions = intersectionForQuadratic(A, B, C, 
                                               ray.mint, ray.maxt, 
                                               t0, t1))) {
      return 0;
   }
   
   return clampAlongZ(ray, mLength, solutions, t0, t1);
}


void 
Cone::computeIntersectionInfo(const Point &p, 
                              PrimitivePatch *pPatch) const
{
   double  phi = atan2(p.y, p.x);
   if (lessThanZero(phi)) {
      phi += (2.0*M_PI);
   }
   
   // Find parametric representation of cone hit
	double u = phi / (2*M_PI);
	double v = p.z / mLength;
   
   // Compute cone dpdu and dpdv
	Vector dpdu(-2*M_PI * p.y, 2*M_PI * p.x, 0);
	Vector dpdv(-p.x / (1.0 - v), -p.y / (1.0 - v), mLength);
   
	// Compute cone dndu and dndv
	Vector d2Pduu = -2*M_PI * 2*M_PI * Vector(p.x, p.y, 0.0);
	Vector d2Pduv = 2*M_PI / (1.0 - v) * Vector(-p.y, -p.x, 0.0);
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
