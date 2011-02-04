

#ifndef CONE_HPP
#define CONE_HPP

#include "point.hpp"
#include "primitive.hpp"
#include "transform.hpp"


class Cone 
   : public Primitive 
{
public:
   Cone(double r = 1.0, double l = 1.0);
   
   virtual ~Cone();
   
private:
      
   size_t computeIntersectionT(const Ray &ray, double *t0, double *t1) const;
   void computeIntersectionInfo(const Point &p, PrimitivePatch *pPatch) const;
   
   // Cone Data
   double mRadius;
   double mLength;
};

#endif // CONE_HPP
