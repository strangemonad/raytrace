

#ifndef CUBE_HPP
#define CUBE_HPP

#include "point.hpp"
#include "primitive.hpp"
#include "transform.hpp"


class Cube 
   : public Primitive 
{
public:
   Cube(double s = 1.0);
   
   virtual ~Cube();
   
   size_t intersect(const Ray &r, 
                    double *tHitNear, 
                    PrimitivePatch *pPatchNear,
                    double *tHitFar = NULL, 
                    PrimitivePatch *pPatchFar = NULL) const;
   
private:
   
   size_t computeIntersectionT(const Ray &ray, double *t0, double *t1) const;
   void computeIntersectionInfo(const Point &p, PrimitivePatch *pPatch) const;
   
   void computeOrthogonal(int axis, int *orthoU, int *orthoV) const;
   double mSideLength;
};

#endif // CUBE_HPP
