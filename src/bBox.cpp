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


#include <algorithm>
#include <cmath>

#include "algebra.hpp"
#include "bBox.hpp"
#include "stats.hpp"


BBox::BBox() 
{
   pMin = Point( INFINITY,  INFINITY,  INFINITY);
   pMax = Point(-INFINITY, -INFINITY, -INFINITY);
}


BBox::BBox(const Point &p) 
   : pMin(p), pMax(p)
{
}


BBox::BBox(const Point &p1, 
           const Point &p2) 
{
   pMin = Point(min(p1.x, p2.x),
                min(p1.y, p2.y),
                min(p1.z, p2.z));
   pMax = Point(max(p1.x, p2.x),
                max(p1.y, p2.y),
                max(p1.z, p2.z));
}


bool 
BBox::overlaps(const BBox &b) const 
{
   bool x = (pMax.x >= b.pMin.x) && (pMin.x <= b.pMax.x);
   bool y = (pMax.y >= b.pMin.y) && (pMin.y <= b.pMax.y);
   bool z = (pMax.z >= b.pMin.z) && (pMin.z <= b.pMax.z);
   return (x && y && z);
}


bool 
BBox::isInside(const Point &pt) const
{
   return (pt.x >= pMin.x && pt.x <= pMax.x &&
           pt.y >= pMin.y && pt.y <= pMax.y &&
           pt.z >= pMin.z && pt.z <= pMax.z);
}


void 
BBox::expand(double delta) 
{
   pMin -= Vector(delta, delta, delta);
   pMax += Vector(delta, delta, delta);
}


double 
BBox::volume() const 
{
   Vector d = pMax - pMin;
   return d[0] * d[1] * d[2];
}


int 
BBox::maximumExtent() const 
{
   Vector diag = pMax - pMin;
   if (diag[0] > diag[1] && diag[0] > diag[2])
      return 0;
   else if (diag[1] > diag[2])
      return 1;
   else
      return 2;
}


bool 
BBox::intersects(const Ray &ray,
                 double *tHitNear,
                 double *tHitFar) const
{
   Stats::sharedStats().boundingBoxIntersections += 1;
   
   if (isDegenerate()) {
      /* 
       * I want degenerates to return false but the degenerate setup makes all
       * other functions work nicely. so special case it here.
       */
      return false;
   }
   
   
   double t0 = ray.mint, t1 = ray.maxt;
   double invRayDir;
   double tNear;
   double tFar;
   
   // Unrolled loop
	for (int i = 0; i < 3; ++i) {
		// Update interval for _i_th bounding box slab
      
      // XXX consider using an approximation to division here... it is a hot spot
      invRayDir = 1.0 / ray.d[i];
      double rayOI = ray.o[i];
      tNear = (pMin[i] - rayOI) * invRayDir;
      tFar  = (pMax[i] - rayOI) * invRayDir;
		// Update parametric interval from slab intersection $t$s
		if (tNear > tFar) std::swap(tNear, tFar);
		t0 = max(tNear,t0);
		t1 = min(tFar, t1);
		if (t0 > t1) 
      {
         return false;
      }
	}
   
   
	if (tHitNear) {
      *tHitNear = t0;
   }
	if (tHitFar) {
      *tHitFar = t1;
   }
	return true;
}
