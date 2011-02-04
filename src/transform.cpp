/*
 * transform.cpp
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

#include "algebra.hpp"
#include "bBox.hpp"
#include "transform.hpp"


// Transform Method Definitions
std::ostream &operator<<(std::ostream &os, const Transform &t) {   
   return os << t.m;
}

Transform Translate(const Vector &delta) {
	Matrix4x4 m, minv;
	m = Matrix4x4(1, 0, 0, delta[0],
                 0, 1, 0, delta[1],
                 0, 0, 1, delta[2],
                 0, 0, 0,       1);
	minv = Matrix4x4(1, 0, 0, -delta[0],
                    0, 1, 0, -delta[1],
                    0, 0, 1, -delta[2],
                    0, 0, 0,        1);
	return Transform(m, minv);
}

Transform Scale(double x, double y, double z) {
	Matrix4x4 m, minv;
	m = Matrix4x4(x, 0, 0, 0,
                 0, y, 0, 0,
                 0, 0, z, 0,
                 0, 0, 0, 1);
	minv = Matrix4x4(1.0/x,     0,     0, 0,
                    0, 1.0/y,     0, 0,
                    0,     0, 1.0/z, 0,
                    0,     0,     0, 1);
	return Transform(m, minv);
}

Transform RotateX(double angle) {
	double sin_t = sinf(Radians(angle));
	double cos_t = cosf(Radians(angle));
	Matrix4x4 m = Matrix4x4(1,     0,      0, 0,
                           0, cos_t, -sin_t, 0,
                           0, sin_t,  cos_t, 0,
                           0,     0,      0, 1);
	return Transform(m, m.transpose());
}
Transform RotateY(double angle) {
	double sin_t = sinf(Radians(angle));
	double cos_t = cosf(Radians(angle));
	Matrix4x4 m = Matrix4x4( cos_t,   0, sin_t, 0,
                            0,   1,     0, 0,
                            -sin_t,   0, cos_t, 0,
                            0,   0,     0, 1);
	return Transform(m, m.transpose());
}

Transform RotateZ(double angle) {
	double sin_t = sinf(Radians(angle));
	double cos_t = cosf(Radians(angle));
	Matrix4x4 m = Matrix4x4(cos_t, -sin_t, 0, 0,
                           sin_t,  cos_t, 0, 0,
                           0,      0, 1, 0,
                           0,      0, 0, 1);
	return Transform(m, m.transpose());
}

Transform Rotate(double angle, const Vector &axis) {
	Vector a = normalize(axis);
	double s = sinf(Radians(angle));
	double c = cosf(Radians(angle));
	double m[4][4];

	m[0][0] = a[0] * a[0] + (1.0 - a[0] * a[0]) * c;
	m[0][1] = a[0] * a[1] * (1.0 - c) - a[2] * s;
	m[0][2] = a[0] * a[2] * (1.0 - c) + a[1] * s;
	m[0][3] = 0;

	m[1][0] = a[0] * a[1] * (1.0 - c) + a[2] * s;
	m[1][1] = a[1] * a[1] + (1.0 - a[1] * a[1]) * c;
	m[1][2] = a[1] * a[2] * (1.0 - c) - a[0] * s;
	m[1][3] = 0;

	m[2][0] = a[0] * a[2] * (1.0 - c) - a[1] * s;
	m[2][1] = a[1] * a[2] * (1.0 - c) + a[0] * s;
	m[2][2] = a[2] * a[2] + (1.0 - a[2] * a[2]) * c;
	m[2][3] = 0;

	m[3][0] = 0;
	m[3][1] = 0;
	m[3][2] = 0;
	m[3][3] = 1;

	Matrix4x4 mat = Matrix4x4(m);
	return Transform(mat, mat.transpose());
}

Transform LookAt(const Point &pos, const Point &look, const Vector &up) {
	double m[4][4];
	// Initialize fourth column of viewing matrix
	m[0][3] = pos[0];
	m[1][3] = pos[1];
	m[2][3] = pos[2];
	m[3][3] = 1;
	// Initialize first three columns of viewing matrix
	Vector dir = normalize(look - pos);
	Vector right = normalize(cross(dir, up));
	Vector newUp = cross(right, dir);
	m[0][0] = right[0];
	m[1][0] = right[1];
	m[2][0] = right[2];
	m[3][0] = 0.0;
	m[0][1] = newUp[0];
	m[1][1] = newUp[1];
	m[2][1] = newUp[2];
	m[3][1] = 0.0;
	m[0][2] = dir[0];
	m[1][2] = dir[1];
	m[2][2] = dir[2];
	m[3][2] = 0.0;
	Matrix4x4 camToWorld = Matrix4x4(m);
	return Transform(camToWorld.inverse(), camToWorld);
}


bool Transform::HasScale() const {
#if 0
	double det = 
      fabsf(m.m[0][0] * (m.m[1][1] * m.m[2][2] - m.m[1][2] * m.m[2][1])) -
      (m.m[0][1] * (m.m[1][0] * m.m[2][2] - m.m[1][2] * m.m[2][0])) +
      (m.m[0][2] * (m.m[1][0] * m.m[2][1] - m.m[1][1] * m.m[2][0]));
	return (det < .999f || det > 1.001f);
#endif
	return false;
}


BBox Transform::operator()(const BBox &b) const {
	const Transform &M = *this;
	BBox ret(M(Point(b.pMin.x, b.pMin.y, b.pMin.z)));
	ret = boxUnion(ret, M(Point(b.pMax.x, b.pMin.y, b.pMin.z)));
	ret = boxUnion(ret, M(Point(b.pMin.x, b.pMax.y, b.pMin.z)));
	ret = boxUnion(ret, M(Point(b.pMin.x, b.pMin.y, b.pMax.z)));
	ret = boxUnion(ret, M(Point(b.pMin.x, b.pMax.y, b.pMax.z)));
	ret = boxUnion(ret, M(Point(b.pMax.x, b.pMax.y, b.pMin.z)));
	ret = boxUnion(ret, M(Point(b.pMax.x, b.pMin.y, b.pMax.z)));
	ret = boxUnion(ret, M(Point(b.pMax.x, b.pMax.y, b.pMax.z)));
	return ret;
}


Transform Transform::operator*(const Transform &t2) const {
	return Transform(m * t2.m, t2.mInv * mInv);
}


bool Transform::SwapsHandedness() const {
	double det = ((m.m[0][0] *
                  (m.m[1][1] * m.m[2][2] -
                   m.m[1][2] * m.m[2][1])) -
                 (m.m[0][1] *
                  (m.m[1][0] * m.m[2][2] -
                   m.m[1][2] * m.m[2][0])) +
                 (m.m[0][2] *
                  (m.m[1][0] * m.m[2][1] -
                   m.m[1][1] * m.m[2][0])));
	return det < 0.0;
}


Transform Orthographic(double znear, double zfar) {
	return Scale(1.0, 1.0, 1.0 / (zfar-znear)) *
		Translate(Vector(0.0, 0.0, -znear));
}


Transform Perspective(double fov, double n, double f) {
	// Perform projective divide
	double inv_denom = 1.0/(f-n);
	Matrix4x4 persp =
      Matrix4x4(1, 0,       0,          0,
                0, 1,       0,          0,
                0, 0, f*inv_denom, -f*n*inv_denom,
                0, 0,       1,          0);
	// Scale to canonical viewing volume
	double invTanAng = 1.0 / tanf(Radians(fov) / 2.0);
	return Scale(invTanAng, invTanAng, 1) * Transform(persp);
}
