#include "stdafx.h"
#include "Camera.h"

using namespace std;

cv::Point2d Camera::projectPoint(const cv::Point3d & pnt) const {
	auto p = toCameraCoords(pnt);
	p.x /= p.z;
	p.y /= p.z;
	double r2 = p.x*p.x + p.y*p.y;
	p.x *= (1.0 + k1*r2 + k2*r2*r2);
	p.y *= (1.0 + k1*r2 + k2*r2*r2);
	return cv::Point2d(K(0,0)*p.x + K(0, 2), K(1, 1)*p.y + K(1, 2));
}

cv::Point3d Camera::toCameraCoords(const cv::Point3d &pnt) const {
	auto p = R*pnt;
	p.x += t[0];
	p.y += t[1];
	p.z += t[2];
	return p;
}

Camera::Camera(const cv::Matx33d &K, const cv::Matx33d &R, const cv::Vec3d &t, double k1, double k2)
	: K(K), R(R), t(t), k1(k1), k2(k2){
}

Camera::Camera(const cv::Matx33d &K, const cv::Affine3d &affine, double k1, double k2)
	: Camera(K, affine.rotation(), affine.translation(), k1, k2) {
}

Camera::Camera(const cv::Matx33d &K, const cv::Vec3d &rvec, const cv::Vec3d &t, double k1, double k2)
	: Camera(K, cv::Affine3d(rvec, t), k1, k2) {
}

Camera::Camera(cv::viz::Viz3d &viz3d, double k1, double k2)
	: Camera(viz3d.getCamera(), viz3d.getViewerPose().inv(), k1, k2) {
}

Camera::Camera(const cv::viz::Camera &cam, const cv::Affine3d &affine, double k1, double k2)
	: K(cv::Matx33d::eye()), k1(k1), k2(k2), R(affine.rotation()), t(affine.translation()) {
	K(0, 0) = cam.getFocalLength()[0];
	K(0, 2) = cam.getPrincipalPoint()[0];
	K(1, 1) = cam.getFocalLength()[1];
	K(1, 2) = cam.getPrincipalPoint()[1];
}

Camera::Camera(double fx, double fy, double cx, double cy, const cv::Matx33d &R, const cv::Vec3d &t, double k1, double k2)
	: K(cv::Matx33d::eye()), k1(k1), k2(k2), R(R), t(t) {
	K(0, 0) = fx;
	K(0, 2) = cx;
	K(1, 1) = fy;
	K(1, 2) = cy;	
}

Camera::Camera(const cv::Matx34d & P, double k1, double k2)
	: k1(k1), k2(k2) {
	cv::decomposeProjectionMatrix(P, K, R, t);
}

Camera::Camera()
	: K(), R(), t(), k1(), k2() {
}

cv::Matx34d Camera::cameraMat() const {
	cv::Matx34d P;
	for (int i = 0; i < 3; ++i)
		P.col(i) = R.col(i);
	P.col(3) = t;
	return P;
}

double Camera::fx() const {
	return K(0, 0);
}

double Camera::fy() const {
	return K(1, 1);
}

double Camera::cx() const {
	return K(0, 2);
}

double Camera::cy() const {
	return K(1, 2);
}

cv::Affine3d Camera::pose() const {
	return cv::Affine3d(R, t);
}
