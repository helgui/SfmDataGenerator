#ifndef CAMERA_H
#define CAMERA_H
#include "stdafx.h"

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
#endif