/*
 * matrix4x4.hpp
 * A4
 *
 * A 4 x 4 matrix that can be used to represent a transformation matrix. This 
 * code is my adaptation of the geometry code presented in "Physically Based 
 * Rendering" by Pharr and Humphreys. and the supplied algebra code.
 *
 * This matrix implementation is dumber in some ways than the one provided in 
 * algebra and a new Transform class is introducted that makes many things 
 * easier to manage.
 *
 * Created by Shawn Morel on 6/24/06.
 * smorel - 20090740
 * Copyright 2006.
 *
 */


#ifndef MATRIX_4X4_HPP
#define MATRIX_4X4_HPP


class Matrix4x4 
{
public:
	
   /*
    * XXX PBRT has a ref counted base class to manage memory a little easier.
    * I'll forgo that for now to make things easier. I may come back to it
    * if I see efficiency problems from going copy crazy but it really should be
    * fine.
    */
   
   Matrix4x4();   
	Matrix4x4(double m_[4][4]);
	Matrix4x4(double t00, double t01, double t02, double t03,
	          double t10, double t11, double t12, double t13,
	          double t20, double t21, double t22, double t23,
	          double t30, double t31, double t32, double t33);
   
   Matrix4x4(const Matrix4x4 &otherMatrix4x4);
   Matrix4x4 &operator=(const Matrix4x4 &otherMatrix);
   
	Matrix4x4 transpose() const;
	Matrix4x4 inverse() const;
   
   
   Matrix4x4 operator*(const Matrix4x4 &otherMatrix) const;

   // XXX could override other operators here but not much need for them.
   
   // XXX might also want begin() and end() for STL.
   
   /*
    * Leave this public for conveniance - equivalently override the appropriate
    * [] operators but that just gets messier. I know I know this isn't proper
    * SE but this isn't meant to be production code just nice and understandable
    * for the project.
    */
	double m[4][4];
};


std::ostream &operator<<(std::ostream& out, const Matrix4x4& M);


#endif // MATRIX_4X4_HPP
