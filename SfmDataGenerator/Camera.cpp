/*
MIT License

Copyright (c) 2017 Oleg Yakovlev

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#include "stdafx.h"
#include "Camera.h"

using namespace std;

void Camera::distort(FltType &x, FltType &y) const {
	FltType r2 = x*x + y*y;
	FltType distMult = 1.0 + k1*r2 + k2*r2*r2;
	x *= distMult;
	y *= distMult;
}

Point2Type Camera::projectPoint(const Point3Type &pnt) const {
	auto p = toCameraCoords(pnt);
	p.x /= p.z;
	p.y /= p.z;
	distort(p.x, p.y);
	return Point2Type(K(0, 0)*p.x + K(0, 2), K(1, 1)*p.y + K(1, 2));
}

Point2Type Camera::projectPointCamCoords(const Point3Type &pnt) const {
	FltType x = pnt.x / pnt.z;
	FltType y = pnt.y / pnt.z;
	distort(x, y);
	return Point2Type(K(0, 0)*x + K(0, 2), K(1, 1)*y + K(1, 2));
}

Point3Type Camera::toCameraCoords(const Point3Type &pnt) const {
	auto p = R*pnt;
	p.x += t[0];
	p.y += t[1];
	p.z += t[2];
	return p;
}

Camera::Camera(const cv::Matx<FltType, 3, 3> &K, const cv::Matx<FltType, 3, 3> &R, const VecType &t, FltType k1, FltType k2)
	: K(K), R(R), t(t), k1(k1), k2(k2) {
}

Camera::Camera(const cv::Matx<FltType, 3, 3> &K, const cv::Affine3<FltType> &affine, FltType k1, FltType k2)
	: Camera(K, affine.rotation(), affine.translation(), k1, k2) {
}

Camera::Camera(const cv::Matx<FltType, 3, 3> &K, const VecType &rvec, const VecType &t, FltType k1, FltType k2)
	: Camera(K, cv::Affine3<FltType>(rvec, t), k1, k2) {
}

Camera::Camera(cv::viz::Viz3d &viz3d, FltType k1, FltType k2)
	: Camera(viz3d.getCamera(), viz3d.getViewerPose().inv(), k1, k2) {
}

Camera::Camera(const cv::viz::Camera &cam, const cv::Affine3<FltType> &affine, FltType k1, FltType k2)
	: K(cv::Matx<FltType, 3, 3>::eye()), k1(k1), k2(k2), R(affine.rotation()), t(affine.translation()) {
	K(0, 0) = cam.getFocalLength()[0];
	K(0, 2) = cam.getPrincipalPoint()[0];
	K(1, 1) = cam.getFocalLength()[1];
	K(1, 2) = cam.getPrincipalPoint()[1];
}

Camera::Camera(FltType fx, FltType fy, FltType cx, FltType cy, const cv::Matx<FltType, 3, 3> &R, const VecType &t, FltType k1, FltType k2)
	: K(cv::Matx<FltType, 3, 3>::eye()), k1(k1), k2(k2), R(R), t(t) {
	K(0, 0) = fx;
	K(0, 2) = cx;
	K(1, 1) = fy;
	K(1, 2) = cy;
}

Camera::Camera(const cv::Matx<FltType, 3, 4> &P, FltType k1, FltType k2)
    : k1(k1), k2(k2) {
    cv::decomposeProjectionMatrix(P, K, R, t);
}

Camera::Camera()
	: K(), R(), t(), k1(), k2() {
}

cv::Matx<FltType, 3, 4> Camera::cameraMat() const {
	cv::Matx<FltType, 3, 4> P;
	for (int i = 0; i < 3; ++i)
		P.col(i) = R.col(i);
	P.col(3) = t;
	return P;
}

FltType Camera::fx() const {
	return K(0, 0);
}

FltType Camera::fy() const {
	return K(1, 1);
}

FltType Camera::cx() const {
	return K(0, 2);
}

FltType Camera::cy() const {
	return K(1, 2);
}

cv::Affine3<FltType> Camera::pose() const {
	return cv::Affine3<FltType>(R, t);
}