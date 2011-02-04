/*
 * transform.hpp
 * A4
 *
 * A object that nicely abstracts a transformation. Internally uses a Matrix4x4
 * but users don't need to be concerned by this and don't have to keep a matrix
 * and it's inverse in sync all the time. This will simplify SceneNode compared
 * to how it was in A3. This code is my adaptation of the transform code 
 * presented in "Physically Based Rendering" by Pharr and Humphreys and 
 * incorporates some of the work I did in A3. I don't consider this code to be 
 * central to the concept of ray tracing.
 *
 */

#ifndef PBRT_TRANSFORM_H
#define PBRT_TRANSFORM_H

#include <iostream>

#include "bBox.hpp"
#include "defs.hpp"
#include "point.hpp"
#include "matrix4x4.hpp"
#include "ray.hpp"
#include "vector.hpp"


class Transform {
public:
	// Transform Public Methods
	Transform() {
		m = mInv = Matrix4x4();
	}
   
	Transform(double mat[4][4]) {
		m = Matrix4x4(mat[0][0],mat[0][1],mat[0][2],mat[0][3],
                    mat[1][0],mat[1][1],mat[1][2],mat[1][3],
                    mat[2][0],mat[2][1],mat[2][2],mat[2][3],
                    mat[3][0],mat[3][1],mat[3][2],mat[3][3]);
		mInv = m.inverse();
	}
   
	Transform(const Matrix4x4 &mat) {
		m = mat;
		mInv = m.inverse();
	}
   
	Transform(const Matrix4x4 &mat,
	          const Matrix4x4 &minv) {
		m = mat;
		mInv = minv;
	}
   
	friend std::ostream &operator<<(std::ostream &, const Transform &);
	Transform getInverse() const {
		return Transform(mInv, m);
	}
   
	bool HasScale() const;
	inline Point operator()(const Point &pt) const;
	inline void operator()(const Point &pt,Point *ptrans) const;
	inline Vector operator()(const Vector &v) const;
	inline void operator()(const Vector &v, Vector *vt) const;
	inline Normal operator()(const Normal &) const;
	inline void operator()(const Normal &, Normal *nt) const;
	inline Ray operator()(const Ray &r) const;
	inline void operator()(const Ray &r, Ray *rt) const;
	
   BBox operator()(const BBox &b) const;
	
   Transform operator*(const Transform &t2) const;
	bool SwapsHandedness() const;

private:
	// Transform Private Data
	Matrix4x4 m, mInv;
};


Transform Translate(const Vector &delta);
Transform Scale(double x, double y, double z);
Transform RotateX(double angle);
Transform RotateY(double angle);
Transform RotateZ(double angle);
Transform Rotate(double angle, const Vector &axis);

Transform LookAt(const Point &pos, const Point &look, const Vector &up);
Transform Orthographic(double znear, double zfar);
Transform Perspective(double fov, double n, double f);


// Transform Inline Functions
inline Point Transform::operator()(const Point &pt) const {
	double x = pt[0], y = pt[1], z = pt[2];
	double xp = m.m[0][0]*x + m.m[0][1]*y + m.m[0][2]*z + m.m[0][3];
	double yp = m.m[1][0]*x + m.m[1][1]*y + m.m[1][2]*z + m.m[1][3];
	double zp = m.m[2][0]*x + m.m[2][1]*y + m.m[2][2]*z + m.m[2][3];
	double wp = m.m[3][0]*x + m.m[3][1]*y + m.m[3][2]*z + m.m[3][3];
   
	ASSERT(wp != 0);
	if (wp == 1.) return Point(xp, yp, zp);
	else          return Point(xp, yp, zp)/wp;
}
inline void Transform::operator()(const Point &pt,
                                  Point *ptrans) const {   
   double x = pt[0], y = pt[1], z = pt[2];
	(*ptrans)[0] = m.m[0][0] * x + m.m[0][1] * y + m.m[0][2] * z + m.m[0][3];
	(*ptrans)[1] = m.m[1][0]*x + m.m[1][1]*y + m.m[1][2]*z + m.m[1][3];
	(*ptrans)[2] = m.m[2][0]*x + m.m[2][1]*y + m.m[2][2]*z + m.m[2][3];
	double wp   = m.m[3][0]*x + m.m[3][1]*y + m.m[3][2]*z + m.m[3][3];
   
   ASSERT(wp != 0);
   if (wp != 1.) *ptrans /= wp;
                                  }
inline Vector Transform::operator()(const Vector &v) const {
   double x = v[0], y = v[1], z = v[2];
   return Vector(m.m[0][0]*x + m.m[0][1]*y + m.m[0][2]*z,
                 m.m[1][0]*x + m.m[1][1]*y + m.m[1][2]*z,
                 m.m[2][0]*x + m.m[2][1]*y + m.m[2][2]*z);
}
inline void Transform::operator()(const Vector &v,
                                  Vector *vt) const {
   double x = v[0], y = v[1], z = v[2];
   (*vt)[0] = m.m[0][0] * x + m.m[0][1] * y + m.m[0][2] * z;
   (*vt)[1] = m.m[1][0] * x + m.m[1][1] * y + m.m[1][2] * z;
   (*vt)[2] = m.m[2][0] * x + m.m[2][1] * y + m.m[2][2] * z;
                                  }
inline Normal Transform::operator()(const Normal &n) const {
   double x = n[0], y = n[1], z = n[2];
	return Normal(mInv.m[0][0]*x + mInv.m[1][0]*y + mInv.m[2][0]*z,
                 mInv.m[0][1]*x + mInv.m[1][1]*y + mInv.m[2][1]*z,
                 mInv.m[0][2]*x + mInv.m[1][2]*y + mInv.m[2][2]*z);
}
inline void Transform::operator()(const Normal &n,
                                  Normal *nt) const {
   double x = n[0], y = n[1], z = n[2];
	(*nt)[0] = mInv.m[0][0]*x + mInv.m[1][0]*y + mInv.m[2][0]*z;
	(*nt)[1] = mInv.m[0][1]*x + mInv.m[1][1]*y + mInv.m[2][1]*z;
	(*nt)[2] = mInv.m[0][2]*x + mInv.m[1][2]*y + mInv.m[2][2]*z;
}
inline Ray Transform::operator()(const Ray &r) const {
	Ray ret(r);
	(*this)(r.o, &ret.o);
	(*this)(r.d, &ret.d);
	ret.mint = r.mint;
	ret.maxt = r.maxt;
 	return ret;
}
inline void Transform::operator()(const Ray &r,
                                  Ray *rt) const {
   (*this)(r.o, &rt->o);
	(*this)(r.d, &rt->d);
   
	rt->mint = r.mint;
	rt->maxt = r.maxt;
}
#endif // PBRT_TRANSFORM_H
