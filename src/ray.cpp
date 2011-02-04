/*
 * ray.cpp
 * A4
 *
 * A ray primitive. This code is my adaptation of the geometry code 
 * presented in "Physically Based Rendering" by Pharr and Humphreys.
 *
 * Created by Shawn Morel on 6/24/06.
 * smorel - 20090740
 * Copyright 2006.
 *
 */


#include <cmath>

#include "ray.hpp"


/*
 * This value is what pbrt uses in for its Ray implementation to avoid errors.
 * Their implementation used floats and I am using doubles so it should be just
 * as good a hack. NOTE: there will always be scenes that and models that have
 * certain scaling that will make this not work and we will end up with problems
 * like fake total internal reflection.
 */
const double RAY_EPSILON = 1e-3f;


/*
 *------------------------------------------------------------------------------
 * 
 * Ray::Ray --
 *     
 *     Constructor.
 *
 *------------------------------------------------------------------------------
 */

Ray::Ray(const Point &origin, 
         const Vector &direction,
         double start, 
         double end)
   : o(origin), 
     d(direction), 
     mint(start), 
     maxt(end)
{
}


/*
 *------------------------------------------------------------------------------
 * 
 * Ray::operator() --
 *     
 *     Evaluate the ray at a specific parameter value. Returns a point for the 
 *     value.
 *
 *------------------------------------------------------------------------------
 */

Point 
Ray::operator()(double t) const 
{
   return o + d * t; 
}
