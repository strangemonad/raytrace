/*
 * vector.hpp
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


#ifndef VECTOR_HPP
#define VECTOR_HPP


#include <cmath>
#include <iostream>

#include "defs.hpp"

class Normal;
class Point;

class Vector 
{

public:
	
   Vector(double _x=0, double _y=0, double _z=0);
	
   explicit Vector(const Point &p);
   explicit Vector(const Normal &n);

   Vector(const Vector &otherVector);
   Vector &operator=(const Vector &otherVector);
   
   bool operator==(const Vector &otherVector) const;

	Vector operator+(const Vector &v) const;
	Vector &operator+=(const Vector &v);
	Vector operator-(const Vector &v) const;
	Vector &operator-=(const Vector &v);
   
	Vector operator*(double scalar) const;
	Vector &operator*=(double scalar);
	Vector operator/(double scalar) const;
	Vector &operator/=(double scalar);
   
	Vector operator-() const;
	
   // XXX should make the params into an enum
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
   
	double lengthSquared() const;
	double length() const;
         
   // Vector private data. Needs to be declared in this order.
   // XXX this should become a coord tripple struct?
	double x, y, z;
   
};


inline std::ostream &
operator<<(std::ostream &out, 
           const Vector &v)
{
	return out << v[0] << ", " << v[1] << ", " << v[2];
}


inline Vector 
operator*(double scalar, 
          const Vector &v) 
{
	return v * scalar;
}


// XX should probably make some of these binary operators member funcs

inline double 
dot(const Vector &v1, 
    const Vector &v2) 
{
	return v1[0] * v2[0] + v1[1] * v2[1] + v1[2] * v2[2];
}


inline double 
fabsDot(const Vector &v1, 
        const Vector &v2) 
{
	return fabs(dot(v1, v2));
}


inline Vector 
cross(const Vector &v1, 
      const Vector &v2) 
{
	return Vector((v1[1] * v2[2]) - (v1[2] * v2[1]),
                 (v1[2] * v2[0]) - (v1[0] * v2[2]),
                 (v1[0] * v2[1]) - (v1[1] * v2[0]));
}


Vector cross(const Vector &v, const Normal &n);
Vector cross(const Normal &n, const Vector &v);


inline Vector 
normalize(const Vector &v) 
{
   // XXX The way it was done in algebra may be more stable.
	return v / v.length();
}


inline void 
coordinateSystem(const Vector &v1,
                 Vector *v2, 
                 Vector *v3) 
{
	if (fabs(v1[0]) > fabs(v1[1])) {
		double invLen = 1.0 / sqrt(v1[0] * v1[0] + v1[2] * v1[2]);
		*v2 = Vector(-v1[2] * invLen, 0.0, v1[0] * invLen);
	}
	else {
		float invLen = 1.0 / sqrt(v1[1] * v1[1] + v1[2] * v1[2]);
		*v2 = Vector(0.0, v1[2] * invLen, -v1[1] * invLen);
	}
	*v3 = cross(v1, *v2);
}


#endif // VECTOR
