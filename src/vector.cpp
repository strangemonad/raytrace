/*
 * vector.cpp
 * A4
 *
 * A 3-space vector. This code is my adaptation of the geometry code 
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
#include "normal.hpp"
#include "point.hpp"
#include "vector.hpp"

// XXX everything needs comments

Vector::Vector(double _x, 
               double _y, 
               double _z)
   : x(_x), 
     y(_y),
     z(_z) 
{
}

 
Vector::Vector(const Point &p)
   : x(p[0]), 
     y(p[1]),
     z(p[2])
{
}


Vector::Vector(const Normal &n)
   : x(n[0]), 
     y(n[1]),
     z(n[2])
{
}


Vector::Vector(const Vector &otherVector)
   : x(otherVector.x),
     y(otherVector.y),
     z(otherVector.z)
{
}


Vector &
Vector::operator=(const Vector &otherVector)
{
   x = otherVector.x;
   y = otherVector.y;
   z = otherVector.z;

   return *this;
}


bool 
Vector::operator==(const Vector &otherVector) const 
{
   return isEqual(x, otherVector.x) && 
          isEqual(y, otherVector.y) && 
          isEqual(z, otherVector.z);
}


Vector 
Vector::operator+(const Vector &v) const
{
   return Vector(x + v.x, y + v.y, z + v.z);
}


Vector & 
Vector::operator+=(const Vector &v) 
{
   x += v.x; y += v.y; z += v.z;
   return *this;
}


Vector 
Vector::operator-(const Vector &v) const 
{
   return Vector(x - v.x, y - v.y, z - v.z);
}


Vector & 
Vector::operator-=(const Vector &v) 
{
   x -= v.x; y -= v.y; z -= v.z;
   return *this;
}


Vector 
Vector::operator*(double scalar) const 
{
   return Vector(scalar * x, scalar * y, scalar * z);
}


Vector &
Vector::operator*=(double scalar) 
{
   x *= scalar; y *= scalar; z *= scalar;
   return *this;
}


Vector 
Vector::operator/(double scalar) const 
{
   ASSERT(!isZero(scalar));
   double inv = 1.0 / scalar;
   return Vector(x * inv, y * inv, z * inv);
}


Vector &
Vector::operator/=(double scalar) 
{
   ASSERT(!isZero(scalar));
   double inv = 1.0 / scalar;
   x *= inv; y *= inv; z *= inv;
   return *this;
}


Vector 
Vector::operator-() const 
{
   return Vector(-x, -y, -z);
}


double 
Vector::lengthSquared() const 
{
   return x*x + y*y + z*z; 
}


double 
Vector::length() const 
{
   return sqrt(lengthSquared());
}


Vector 
cross(const Vector &v,
      const Normal &n) 
{
	return Vector((v[1] * n[2]) - (v[2] * n[1]),
                 (v[2] * n[0]) - (v[0] * n[2]),
                 (v[0] * n[1]) - (v[1] * n[0]));
}


Vector 
cross(const Normal &n, 
      const Vector &v) 
{
	return Vector((n[1] * v[2]) - (n[2] * v[1]),
                 (n[2] * v[0]) - (n[0] * v[2]),
                 (n[0] * v[1]) - (n[1] * v[0]));
}
