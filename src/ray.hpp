/*
 * ray.hpp
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


#ifndef RAY_HPP
#define RAY_HPP

#include <iostream>

#include "point.hpp"
#include "vector.hpp"


/*
 * Lower bound on the ray parameterization. Prevents certain errors from
 * roundoff during ray object intersection.
 */
extern const double RAY_EPSILON;


class Ray 
{
public:
	Ray(const Point &origin = Point(0.0, 0.0, 0.0), 
       const Vector &direction = Vector(0.0, 0.0, 0.0),
       double start = RAY_EPSILON, 
       double end = INFINITY);
   
   Point operator()(double t) const;
      
   // Public variables
	Point o;
	Vector d;
   
   // Define these mutable to allow these to be changed for a const Ray ref.
   mutable double mint;
   mutable double maxt; // clamping of the parametric range.
   
	// Could have a time value for motion blur but let's not worry about that.
};


inline std::ostream &
operator<<(std::ostream &out, 
           const Ray &r)
{
	out << "origin: " << r.o 
       << " direction: " << r.d 
       << " range [" << r.mint << "," << r.maxt << "]";
   
	return out;
}

#endif // RAY_HPP
