/*
 * matrix.hpp
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


#include <algorithm>
#include <cmath>
#include <iostream>

#include "defs.hpp"
#include "matrix4x4.hpp"


Matrix4x4::Matrix4x4()
{
	// Construct an identity matrix
   //std::fill(m, m+16, 0.0);
   bzero(m, 16*sizeof(double));
   m[0][0] = 1.0;
   m[1][1] = 1.0;
   m[2][2] = 1.0;
   m[3][3] = 1.0;
}


Matrix4x4::Matrix4x4(double m_[4][4])
{
   // std::copy(m_, m_+16, m); ? not sure which is faster
   memcpy(m, m_, 16*sizeof(double));
}


Matrix4x4::Matrix4x4(const Matrix4x4 &otherMatrix)
{
   // std::copy(Matrix4x4.m, Matrix4x4.m+16, m); ? not sure which is faster
   memcpy(m, otherMatrix.m, 16*sizeof(double));
}


Matrix4x4& 
Matrix4x4::operator=(const Matrix4x4 &otherMatrix)
{
   // std::copy(otherMatrix.m, otherMatrix.v+16, m); ? not sure which is faster
   memcpy(m, otherMatrix.m, 16*sizeof(double));
   return *this;
}


Matrix4x4::Matrix4x4(double t00, double t01, double t02, double t03,
                     double t10, double t11, double t12, double t13,
                     double t20, double t21, double t22, double t23,
                     double t30, double t31, double t32, double t33) 
{
	m[0][0] = t00; m[0][1] = t01; m[0][2] = t02; m[0][3] = t03;
	m[1][0] = t10; m[1][1] = t11; m[1][2] = t12; m[1][3] = t13;
	m[2][0] = t20; m[2][1] = t21; m[2][2] = t22; m[2][3] = t23;
	m[3][0] = t30; m[3][1] = t31; m[3][2] = t32; m[3][3] = t33;
}


Matrix4x4
Matrix4x4::transpose() const 
{
   return Matrix4x4(m[0][0], m[1][0], m[2][0], m[3][0],
                    m[0][1], m[1][1], m[2][1], m[3][1],
                    m[0][2], m[1][2], m[2][2], m[3][2],
                    m[0][3], m[1][3], m[2][3], m[3][3]);
}


Matrix4x4
Matrix4x4::inverse() const 
{
	int indxc[4], indxr[4];
	int ipiv[4] = { 0, 0, 0, 0 };
	double minv[4][4];
	memcpy(minv, m, 4*4*sizeof(double));
	for (int i = 0; i < 4; i++) {
		int irow = -1, icol = -1;
		double big = 0.;
		// Choose pivot
		for (int j = 0; j < 4; j++) {
			if (ipiv[j] != 1) {
				for (int k = 0; k < 4; k++) {
					if (ipiv[k] == 0) {
						if (fabs(minv[j][k]) >= big) {
							big = double(fabsf(minv[j][k]));
							irow = j;
							icol = k;
						}
					}
					else if (ipiv[k] > 1)
						WARN("Singular matrix in MatrixInvert");
				}
			}
		}
      
		++ipiv[icol];
		// Swap rows _irow_ and _icol_ for pivot
		if (irow != icol) {
			for (int k = 0; k < 4; ++k)
            std::swap(minv[irow][k], minv[icol][k]);
		}
		indxr[i] = irow;
		indxc[i] = icol;
		if (minv[icol][icol] == 0.)
			WARN("Singular matrix in MatrixInvert");
		// Set $m[icol][icol]$ to one by scaling row _icol_ appropriately
		double pivinv = 1.f / minv[icol][icol];
		minv[icol][icol] = 1.f;
		for (int j = 0; j < 4; j++) {
			minv[icol][j] *= pivinv;
      }
      
		// Subtract this row from others to zero out their columns
		for (int j = 0; j < 4; j++) {
			if (j != icol) {
				double save = minv[j][icol];
				minv[j][icol] = 0;
				for (int k = 0; k < 4; k++) {
					minv[j][k] -= minv[icol][k]*save;
            }
			}
		}
	}
   
	// Swap columns to reflect permutation
	for (int j = 3; j >= 0; j--) {
		if (indxr[j] != indxc[j]) {
			for (int k = 0; k < 4; k++)
            std::swap(minv[k][indxr[j]], minv[k][indxc[j]]);
		}
	}
	return Matrix4x4(minv);
}


Matrix4x4 
Matrix4x4::operator*(const Matrix4x4& otherMatrix) const
{
   double r[4][4];
   for (int i = 0; i < 4; ++i) {
      for (int j = 0; j < 4; ++j) {
         r[i][j] = m[i][0] * otherMatrix.m[0][j] +
                   m[i][1] * otherMatrix.m[1][j] +
                   m[i][2] * otherMatrix.m[2][j] +
                   m[i][3] * otherMatrix.m[3][j];
      }
   }
   return Matrix4x4(r);
}


// Non member functions


std::ostream &
operator<<(std::ostream& out, 
           const Matrix4x4& M)

{
   return out  << "[" << M.m[0][0] << " " << M.m[0][1] << " " 
                      << M.m[0][2] << " " << M.m[0][3] << "]" << std::endl
               << "[" << M.m[1][0] << " " << M.m[1][1] << " " 
                      << M.m[1][2] << " " << M.m[1][3] << "]" << std::endl
               << "[" << M.m[2][0] << " " << M.m[2][1] << " " 
                      << M.m[2][2] << " " << M.m[2][3] << "]" << std::endl
               << "[" << M.m[3][0] << " " << M.m[3][1] << " " 
                      << M.m[3][2] << " " << M.m[3][3] << "]";
}
