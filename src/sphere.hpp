

#ifndef SPHERE_HPP
#define SPHERE_HPP

#include "point.hpp"
#include "primitive.hpp"
#include "transform.hpp"


class Sphere 
   : public Primitive 
{
public:
   Sphere(double r = 1.0);
   
   virtual ~Sphere();
   
private:
      
   size_t computeIntersectionT(const Ray &ray, double *t0, double *t1) const;
   void computeIntersectionInfo(const Point &p, PrimitivePatch *pPatch) const;
   
   double mRadius;
};

#endif // SPHERE_HPP
