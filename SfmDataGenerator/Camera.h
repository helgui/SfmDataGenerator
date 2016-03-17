#ifndef CAMERA_H
#define CAMERA_H
#include "stdafx.h"
#include "Common.h"

class Camera {
public:
	Camera(cv::viz::Viz3d & viz3d, double k1 = 0.0, double k2 = 0.0);
	Camera(const cv::viz::Camera &cam, const cv::Affine3d &affine, double k1 = 0.0, double k2 = 0.0);
	Camera(const cv::Matx33d &K, const cv::Affine3d &affine, double k1 = 0.0, double k2 = 0.0);
	Camera(const cv::Matx33d &K, const cv::Matx33d &R, const cv::Vec3d &t, double k1 = 0.0, double k2 = 0.0);
	Camera(const cv::Matx33d &K, const cv::Vec3d &rvec, const cv::Vec3d &t, double k1 = 0.0, double k2 = 0.0);
	Camera(double fx, double fy, double cx, double cy, const cv::Matx33d &R, const cv::Vec3d &t, double k1 = 0.0, double k2 = 0.0);
	Camera();
	Camera(const cv::Matx34d & P, double k1 = 0.0, double k2 = 0.0);
	cv::Point3d toCameraCoords(const cv::Point3d &pnt) const;
	cv::Point2d projectPoint(const cv::Point3d &pnt) const;
	cv::Matx34d cameraMat() const;
	double fx() const;
	double fy() const;
	double cx() const;
	double cy() const;
	cv::Affine3d pose() const;
	cv::Vec3d t;
	cv::Matx33d R;
	cv::Matx33d K;
	double k1, k2;
};

static void write(cv::FileStorage &fs, const cv::String &, const Camera &cam) {
	fs << "[";
	for (int i = 0; i < 9; ++i)
		fs << cam.K.val[i];
	for (int i = 0; i < 9; ++i)
		fs << cam.R.val[i];
	for (int i = 0; i < 3; ++i)
		fs << cam.t[i];
	fs << cam.k1 << cam.k2;
	fs << "]";
}

static void read(const cv::FileNode &node, Camera &cam, const Camera &defaultCam) {
	auto it = node.begin();
	for (int i = 0; i < 9; ++i) {
		(*it) >> cam.K.val[i];
		++it;
	}
	for (int i = 0; i < 9; ++i) {
		(*it) >> cam.R.val[i];
		++it;
	}
	for (int i = 0; i < 3; ++i) {
		(*it) >> cam.t[i];
		++it;
	}
	(*it) >> cam.k1;
	++it;
	(*it) >> cam.k2;
}
#endif