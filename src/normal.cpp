/*
 * normal.cpp
 * A4
 *
 * A normal vector. This code is my adaptation of the geometry code 
 * presented in "Physically Based Rendering" by Pharr and Humphreys.
 *
 * Although a normal is really a vector having a special class allows to 
 * properly apply transformations.
 *
 * Created by Shawn Morel on 6/24/06.
 * smorel - 20090740
 * Copyright 2006.
 *
 */

#include "defs.hpp"
#include "normal.hpp"
#include "point.hpp"
#include "vector.hpp"


Normal::Normal(double x, 
               double y, 
               double z)
   : v(Vector(x, y, z))
{
}


Normal::Normal(const Point &p)
   : v(Vector(p))
{
}


Normal::Normal(const Vector &v_)
   : v(v_)
{
}


Normal::Normal(const Normal &otherNormal)
   : v(otherNormal.v)
{
}


Normal &
Normal::operator=(const Normal &otherNormal)
{
   v = otherNormal.v;
   return *this;
}


bool 
Normal::operator==(const Normal &otherNormal) const 
{
   return v == otherNormal.v;
}


Normal 
Normal::operator+(const Normal &n) const
{
   return Normal(v + n.v); 
}


Normal & 
Normal::operator+=(const Normal &n)
{
   v += n.v;
   return *this;
}


Normal 
Normal::operator-(const Normal &n) const
{
   return Normal(v - n.v); 
}


Normal &
Normal::operator-=(const Normal &n)
{
   v -= n.v;
   return *this;
}


Normal 
Normal::operator*(double scalar) const
{
   return Normal(v * scalar); 

}


Normal &
Normal::operator*=(double scalar)
{
   v *= scalar;
   return *this;
}


Normal 
Normal::operator/(double scalar) const
{
   return Normal(v / scalar); 
}


Normal &
Normal::operator/=(double scalar)
{
   v /= scalar;
   return *this;
}


Normal 
Normal::operator-() const
{
   return Normal(-v);
}


double 
Normal::operator[](size_t coord) const
{
   return v[coord];
}


double &
Normal::operator[](size_t coord)
{
   return v[coord];
}


double 
Normal::lengthSquared() const
{
   return v.lengthSquared();
}


double 
Normal::length() const
{
   return v.length();
}
