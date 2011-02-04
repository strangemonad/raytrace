

#ifndef CYLINDER_HPP
#define CYLINDER_HPP

#include "point.hpp"
#include "primitive.hpp"
#include "transform.hpp"


class Cylinder 
   : public Primitive 
{
public:
   Cylinder(double r = 1.0, double l = 1.0);
   
   virtual ~Cylinder();
   
private:
      
   size_t computeIntersectionT(const Ray &ray, double *t0, double *t1) const;
   void computeIntersectionInfo(const Point &p, PrimitivePatch *pPatch) const;
   
   double mRadius;
   double mLength;
   
};

#endif // CYLINDER_HPP
