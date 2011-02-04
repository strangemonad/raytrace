

#ifndef PARABOLOID_HPP
#define PARABOLOID_HPP

#include "point.hpp"
#include "primitive.hpp"
#include "transform.hpp"


class Paraboloid 
   : public Primitive 
{
public:
   Paraboloid(double r = 1.0, double l = 1.0);
   
   virtual ~Paraboloid();
   
private:
      
   size_t computeIntersectionT(const Ray &ray, double *t0, double *t1) const;
   void computeIntersectionInfo(const Point &p, PrimitivePatch *pPatch) const;
   
   // Cone Data
   double mRadius;
   double mLength;
};

#endif // PARABOLOID_HPP
