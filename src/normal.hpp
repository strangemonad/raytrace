/*
 * normal.hpp
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


#ifndef NORMAL_HPP
#define NORMAL_HPP

#include <cmath>
#include <iostream>

#include "vector.hpp"

class Normal
{
public:
   Normal(double _x=0, double _y=0, double _z=0);
	
   explicit Normal(const Point &p);
   explicit Normal(const Vector &v);
   
   Normal(const Normal &otherNormal);
   Normal &operator=(const Normal &otherNormal);
   
   bool operator==(const Normal &otherNormal) const;
   
	Normal operator+(const Normal &n) const;
	Normal &operator+=(const Normal &n);
	Normal operator-(const Normal &n) const;
	Normal &operator-=(const Normal &n);
   
	Normal operator*(double scalar) const;
	Normal &operator*=(double scalar);
	Normal operator/(double scalar) const;
	Normal &operator/=(double scalar);
   
	Normal operator-() const;
	
   // XXX should make the params into an enum
   double operator[](size_t coord) const;
	double &operator[](size_t coord);
   
   double lengthSquared() const;
	double length() const;
   	
private:
   Vector v;
};



inline std::ostream &
operator<<(std::ostream &out, 
           const Normal &n)
{
	return out << n[0] << ", " << n[1] << ", " << n[2];
}

inline Normal 
operator*(double scalar, 
          const Normal &n) 
{
	return n * scalar;
}


inline Normal 
normalize(const Normal &n) 
{
	// XXX The way it was done in algebra may be more stable.
   return n / n.length();
}


inline double 
dot(const Normal &n, 
    const Vector &v) 
{
	return n[0] * v[0] + n[1] * v[1] + n[2] * v[2];
}


inline double 
dot(const Vector &v,
    const Normal &n) 
{
   return v[0] * n[0] + v[1] * n[1] + v[2] * n[2];
}


inline double 
dot(const Normal &n1,
    const Normal &n2) 
{
   return n1[0] * n2[0] + n1[1] * n2[1] + n1[2] * n2[2];
}


inline double 
fabsDot(const Normal &n, 
        const Vector &v)
{
	return fabs(dot(n, v));
}


inline double 
fabsDot(const Vector &v,
        const Normal &n)
{
	return fabs(dot(v, n));
}


inline double 
fabsDot(const Normal &n1, 
        const Normal &n2) 
{
	return fabs(dot(n1, n2));
}


#endif // NORMAL_HPP
