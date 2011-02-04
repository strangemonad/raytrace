/*
 * bBox.hpp
 * A5
 *
 * A BBox primitive. This code is my adaptation of the geometry code 
 * presented in "Physically Based Rendering" by Pharr and Humphreys.
 *
 * Created by Shawn Morel on 7/16/06.
 * smorel - 20090740
 * Copyright 2006.
 *
 */


#ifndef B_BOX_HPP
#define B_BOX_HPP

#include <iostream>

#include "algebra.hpp"
#include "point.hpp"
#include "ray.hpp"

class BBox 
{
public:
	BBox();
	BBox(const Point &p);
	BBox(const Point &p1, const Point &p2);
   
   inline bool isDegenerate(void) const
   {
      return pMax.x < pMin.x && pMax.y < pMin.y && pMax.z < pMin.z;
   }
   
	bool overlaps(const BBox &b) const;
	bool isInside(const Point &pt) const;
   
	void expand(double delta);
	double volume() const;
	int maximumExtent() const;
   
   
	bool intersects(const Ray &ray,
	                double *tHitNear = NULL,
                   double *tHitFar = NULL) const;
	
   
   // BBox Public Data
	Point pMin, pMax;
};


inline std::ostream &
operator<<(std::ostream &out, 
           const BBox &b)
{
	return out << "BBox: [" << b.pMin << " ; " << b.pMax << "]";
}


inline BBox 
boxUnion(const BBox &b, const Point &p)
{
   BBox ret = b;
   ret.pMin.x = min(b.pMin.x, p.x);
   ret.pMin.y = min(b.pMin.y, p.y);
   ret.pMin.z = min(b.pMin.z, p.z);
   
   ret.pMax.x = max(b.pMax.x, p.x);
   ret.pMax.y = max(b.pMax.y, p.y);
   ret.pMax.z = max(b.pMax.z, p.z);
   return ret;
}

inline BBox 
boxUnion(const BBox &b, const BBox &b2)
{
   BBox ret;
   ret.pMin.x = min(b.pMin.x, b2.pMin.x);
   ret.pMin.y = min(b.pMin.y, b2.pMin.y);
   ret.pMin.z = min(b.pMin.z, b2.pMin.z);
   ret.pMax.x = max(b.pMax.x, b2.pMax.x);
   ret.pMax.y = max(b.pMax.y, b2.pMax.y);
   ret.pMax.z = max(b.pMax.z, b2.pMax.z);
   return ret;
}

inline BBox 
boxIntersection(const BBox &b, const BBox &b2)
{
   BBox ret;
   ret.pMin.x = max(b.pMin.x, b2.pMin.x);
   ret.pMin.y = max(b.pMin.y, b2.pMin.y);
   ret.pMin.z = max(b.pMin.z, b2.pMin.z);
   ret.pMax.x = min(b.pMax.x, b2.pMax.x);
   ret.pMax.y = min(b.pMax.y, b2.pMax.y);
   ret.pMax.z = min(b.pMax.z, b2.pMax.z);
   return ret;
}

#endif // B_BOX_HPP
