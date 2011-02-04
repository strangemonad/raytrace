

#include <cmath>

#include "algebra.hpp"
#include "defs.hpp"
#include "point.hpp"
#include "sphere.hpp"


Sphere::Sphere(double r)
   : Primitive(BBox(Point(-r, -r, -r), Point(r, r, r))),
     mRadius(r)
{
}


Sphere::~Sphere()
{
}


size_t 
Sphere::computeIntersectionT(const Ray &ray, 
                             double *t0, 
                             double *t1) const
{
   ASSERT(t0);
   ASSERT(t1);
   
   /*
    * A, B, and C determined by substituting the parametric eqn for the ray
    * into the implicit eqn for a this sphere.
    */
	double A = ray.d[0] * ray.d[0] + 
              ray.d[1]*ray.d[1] + 
              ray.d[2]*ray.d[2];
	double B = 2 * (ray.d[0]*ray.o[0] + 
                   ray.d[1]*ray.o[1] + 
                   ray.d[2]*ray.o[2]);
	double C = ray.o[0]*ray.o[0] + 
              ray.o[1]*ray.o[1] + 
              ray.o[2]*ray.o[2] - mRadius*mRadius;
   
   return intersectionForQuadratic(A, B, C, ray.mint, ray.maxt, t0, t1);
}


void 
Sphere::computeIntersectionInfo(const Point &p, 
                                PrimitivePatch *pPatch) const
{
   double phi = atan2(p.y, p.x);
   if (lessThanZero(phi)) {
      phi += (2.0*M_PI);
   }
   
   double u = phi / (2.0*M_PI);
   
   double theta = acos(p.z / mRadius);
   double v = theta / M_PI;
   
   
   // Compute sphere dpdu and dpdv
   double cosphi, sinphi;
   Vector dpdu, dpdv;
   double zradius = sqrtf(p.x*p.x + p.y*p.y);
   if (zradius == 0)
   {
      // Handle hit at degenerate parameterization point
      cosphi = 0;
      sinphi = 1;
      dpdv = M_PI * Vector(p.z * cosphi, 
                           p.z * sinphi, 
                           -mRadius * sinf(theta));
      Vector norm = Vector(p);
      dpdu = cross(dpdv, norm);
   }
   else
   {
      double invzradius = 1.0 / zradius;
      cosphi = p.x * invzradius;
      sinphi = p.y * invzradius;
      dpdu = Vector(-2*M_PI * p.y, 2*M_PI * p.x, 0);
      dpdv = (M_PI) * Vector(p.z * cosphi, 
                             p.z * sinphi,
                             -mRadius * sinf(theta));
   }
   
   
   // Compute sphere dndu and dndv using Weingarten equations.
   Vector d2Pduu = -2 * M_PI * 2 * M_PI * Vector(p.x,p.y,0);
   Vector d2Pduv = M_PI * p.z * 2*M_PI *Vector(-sinphi, cosphi, 0.);
   Vector d2Pdvv = M_PI * M_PI * Vector(p.x, p.y, p.z);
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
