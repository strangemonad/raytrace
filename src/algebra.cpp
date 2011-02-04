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


#include "algebra.hpp"


const double MACHINE_EPSILON  = 1e-9;


bool 
isEqual(double a, double b)
{
   /*
    * This approach will not work for all numeric problems but it should be OK
    * for our purposes. This is what was suggested on 
    * http://c-faq.com/fp/fpequal.html
    */
   
   double c = fabs(a);
	double d = fabs(b);
   
	d = max(c, d);
   return (d == 0.0) ? true : (fabs(a - b) / d) <= MACHINE_EPSILON;
}


bool 
isZero(double a)
{
   return a > -MACHINE_EPSILON && a < MACHINE_EPSILON;
}

bool 
lessThanZero(double a)
{
   return a < MACHINE_EPSILON;
}
