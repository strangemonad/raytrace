//---------------------------------------------------------------------------
//
// CS488 -- Introduction to Computer Graphics
//
// algebra.hpp/algebra.cpp
//
// Classes and functions for manipulating points, vectors, and matrices. 
// You probably won't need to modify anything in these
// two files.
//
// University of Waterloo Computer Graphics Lab / 2003
//
//---------------------------------------------------------------------------

#ifndef CS488_ALGEBRA_HPP
#define CS488_ALGEBRA_HPP

#include <iostream>
#include <algorithm>
#include <cmath>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

extern const double MACHINE_EPSILON;

bool isEqual(double a, double b);
bool isLessThan(double a, double b);
bool isZero(double a);

bool lessThanZero(double a);


inline double 
min(double a, double b)
{
   double c = (a < b) ? a : b;   
   if (c != c) {
      // Handle case min(x, NaN)
      return a;
   }
   return c;
}


inline double 
max(double a, double b)
{
   
   double c = (a > b) ? a : b;   
   if (c != c) {
      // Handle case max(x, NaN)
      return a;
   }
   return c;
}

inline double Radians(double deg) {
	return ((double)M_PI/180.0) * deg;
}


inline double Degrees(double rad) {
	return (180.0/(double)M_PI) * rad;
}

#endif // CS488_ALGEBRA_HPP
