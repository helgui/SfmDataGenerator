/*
	Copyright (c) 2016 Oleg Yakovlev
	This file is a part of SfmDataGenerator software
*/
#ifndef CAMERA_H
#define CAMERA_H
#include "stdafx.h"

class Camera {
public:
	/*Extracts camera params from 3D Visualizer*/
	Camera(cv::viz::Viz3d & viz3d, double k1 = 0.0, double k2 = 0.0);
	/*Contructs camera from viz::Camera and pose*/
	Camera(const cv::viz::Camera &cam, const cv::Affine3d &affine, double k1 = 0.0, double k2 = 0.0);
	/*Constructs camera from intrinsics matrix and pose*/
	Camera(const cv::Matx33d &K, const cv::Affine3d &affine, double k1 = 0.0, double k2 = 0.0);
	/*Constructs camera from instrinsics matrix, rotation matrix and translation vector*/
	Camera(const cv::Matx33d &K, const cv::Matx33d &R, const cv::Vec3d &t, double k1 = 0.0, double k2 = 0.0);
	/*Constructs camera from intrinsics matrix, rotation and translation vectors*/
	Camera(const cv::Matx33d &K, const cv::Vec3d &rvec, const cv::Vec3d &t, double k1 = 0.0, double k2 = 0.0);
	/*Construtcs camera from intrinsic parameters, rotation matrix and tranlation vector*/
	Camera(double fx, double fy, double cx, double cy, const cv::Matx33d &R, const cv::Vec3d &t, double k1 = 0.0, double k2 = 0.0);
	/*Default constructor*/
	Camera();
	/*Constructs camera by a 3x4 projection matrix*/
	Camera(const cv::Matx34d & P, double k1 = 0.0, double k2 = 0.0);
	/*Transforms a point in worlds cooridnate system to camera coordinate systems*/
	cv::Point3d toCameraCoords(const cv::Point3d &pnt) const;
	/*Computes point projection on image*/
	cv::Point2d projectPoint(const cv::Point3d &pnt) const;
	/*Computes point projection on image (point is in camera coords)*/
	cv::Point2d projectPointCamCoords(const cv::Point3d &pnt) const; 
	/*Returns 3x4 camera projection matrix*/
	cv::Matx34d cameraMat() const;
	/*Returns focal length along X axis (in pixels)*/
	double fx() const;
	/*Returns focal length along Y axis (in pixels)*/
	double fy() const;
	/*Returns X coordinate of the principal point*/
	double cx() const;
	/*Returns Y coordinate of the principal point*/
	double cy() const;
	/*Returns camera pose as cv::Affine3d*/
	cv::Affine3d pose() const;
	/*Camera translation*/
	cv::Vec3d t;
	/*Camera rotation*/
	cv::Matx33d R;
	/*Camera intrinsics
		(fx  0 cx)
		( 0 fy cy)
		( 0  0  1)
	*/
	cv::Matx33d K;
	/*Radial distortion coefficients*/
	double k1, k2;
private:
	void distort(double &x, double &y) const;
};

/*Serialization interface*/
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
/*Deserialization interface*/
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