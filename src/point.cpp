/*
 * point.cpp
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


#include "algebra.hpp"
#include "defs.hpp"
#include "point.hpp"


// XXX All of these need proper commenting.

Point::Point(double _x, 
             double _y, 
             double _z)
   : x(_x), 
     y(_y), 
     z(_z) 
{
}


Point::Point(const Vector &v)
   : x(v[0]), 
     y(v[1]),
     z(v[2])
{
}


Point::Point(const Normal &n)
   : x(n[0]), 
     y(n[1]),
     z(n[2])
{
}


Point::Point(const Point &otherPoint)
   : x(otherPoint.x),
     y(otherPoint.y),
     z(otherPoint.z)
{
}


Point &
Point::operator=(const Point &otherPoint)
{
   x = otherPoint.x;
   y = otherPoint.y;
   z = otherPoint.z;
   
   return *this;
}


bool 
Point::operator==(const Point &otherPoint) const
{
   return isEqual(x, otherPoint.x) && 
          isEqual(y, otherPoint.y) && 
          isEqual(z, otherPoint.z);
}


Point 
Point::operator+(const Vector &v) const 
{
   return Point(x + v[0], y + v[1], z + v[2]);
}


Point &
Point::operator+=(const Vector &v) 
{
   x += v[0]; y += v[1]; z += v[2];
   return *this;
}


Point 
Point::operator-(const Vector &v) const 
{
   return Point(x - v[0], y - v[1], z - v[2]);
}


Point &
Point::operator-=(const Vector &v) 
{
   x -= v[0]; y -= v[1]; z -= v[2];
   return *this;
}


Point 
Point::operator*(double scalar) const 
{
   return Point(scalar*x, scalar*y, scalar*z);
}


Point &
Point::operator*=(double scalar)
{
   x *= scalar; y *= scalar; z *= scalar;
   return *this;
}


Point 
Point::operator/(double scalar) const
{
   double inv = 1.0/scalar;
   return Point(inv*x, inv*y, inv*z);
}


Point &
Point::operator/=(double scalar) 
{
   ASSERT(!isZero(scalar));
   double inv = 1.0/scalar;
   x *= inv; y *= inv; z *= inv;
   return *this;
}


Point 
Point::operator+(const Point &p) const 
{
   return Point(x + p.x, y + p.y, z + p.z);
}


Point &
Point::operator+=(const Point &p) 
{
   x += p.x; y += p.y; z += p.z;
   return *this;
}


Vector 
Point::operator-(const Point &p) const 
{
   return Vector(x - p.x, y - p.y, z - p.z);
}
