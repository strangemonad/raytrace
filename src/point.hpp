/*
 * point.hpp
 * A4
 *
 * A point primitive. This code is my adaptation of the geometry code 
 * presented in "Physically Based Rendering" by Pharr and Humphreys and the 
 * supplied algebra code.
 *
 * Created by Shawn Morel on 6/24/06.
 * smorel - 20090740
 * Copyright 2006.
 *
 */


#ifndef POINT_HPP
#define POINT_HPP

#include <iostream>

#include "defs.hpp"
#include "vector.hpp"
#include "normal.hpp"

class Point 
{
public:
	Point(double _x=0.0, double _y=0.0, double _z=0.0);
   
   explicit Point(const Vector &v);
   explicit Point(const Normal &n);
   
   Point(const Point &otherPoint);
   Point &operator=(const Point &otherPoint);
   
   bool operator==(const Point &otherPoint) const;
   
   Point operator+(const Vector &v) const;
   Point &operator+=(const Vector &v);
   Point operator-(const Vector &v) const;
   Point &operator-=(const Vector &v);
   
   Point operator* (double scalar) const;
	Point &operator*=(double scalar);
	Point operator/ (double scalar) const;
	Point &operator/=(double scalar);

   Point operator+(const Point &p) const;
   Point &operator+=(const Point &p);
   
	Vector operator-(const Point &p) const;
   
   // XXX consider making an emun to ask for each point coord by name.
	inline double operator[](size_t coord) const
   {
      ASSERT(coord <= 2);
      return (&x)[coord];
   }
   
	inline double &operator[](size_t coord)
   {
      ASSERT(coord <= 2);
      return (&x)[coord]; 
   }
	
   // Point public Data... must be declared in this order.
	double x, y, z;
};


inline double 
Distance(const Point &p1, 
         const Point &p2) 
{
	return (p1 - p2).length();
}


inline double 
DistanceSquared(const Point &p1, 
                const Point &p2) 
{
	return (p1 - p2).lengthSquared();
}


inline std::ostream &
operator<<(std::ostream &out, 
           const Point &p) 
{
	out << "(" << p[0] << ", " << p[1] << ", " << p[2] << ")";
	
   return out;
}


inline Point 
operator*(double scalar, 
          const Point &p)
{
	return p*scalar;
}

#endif // POINT_HPP
