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

#ifndef CAMERA_H
#define CAMERA_H
#include "stdafx.h"
#include "Common.h"

class Camera {
public:
	/*Extracts camera params from 3D Visualizer*/
	Camera(cv::viz::Viz3d & viz3d, FltType k1 = 0.0, FltType k2 = 0.0);
	
	/*Contructs camera from viz::Camera and pose*/
	Camera(const cv::viz::Camera &cam, const cv::Affine3<FltType> &affine, FltType k1 = 0.0, FltType k2 = 0.0);
	
	/*Constructs camera from intrinsics matrix and pose*/
	Camera(const cv::Matx<FltType, 3, 3> &K, const cv::Affine3<FltType> &affine, FltType k1 = 0.0, FltType k2 = 0.0);
	
	/*Constructs camera from instrinsics matrix, rotation matrix and translation vector*/
	Camera(const cv::Matx<FltType, 3, 3> &K, const cv::Matx<FltType, 3, 3> &R, const VecType &t, FltType k1 = 0.0, FltType k2 = 0.0);
	
	/*Constructs camera from intrinsics matrix, rotation and translation vectors*/
	Camera(const cv::Matx<FltType, 3, 3> &K, const VecType &rvec, const VecType &t, FltType k1 = 0.0, FltType k2 = 0.0);
	
	/*Construtcs camera from intrinsic parameters, rotation matrix and tranlation vector*/
	Camera(FltType fx, FltType fy, FltType cx, FltType cy, const cv::Matx<FltType, 3, 3> &R, const VecType &t, FltType k1 = 0.0, FltType k2 = 0.0);
	
	/*Default constructor*/
	Camera();
	
	/*Constructs camera by a 3x4 projection matrix*/
	Camera(const cv::Matx<FltType, 3, 4> & P, FltType k1 = 0.0, FltType k2 = 0.0);
	
	/*Transforms a point in worlds cooridnate system to camera coordinate system*/
	Point3Type toCameraCoords(const Point3Type &pnt) const;
	
	/*Computes point projection on image*/
	Point2Type projectPoint(const Point3Type &pnt) const;

	/*Computes point projection on image (point is in camera coords)*/
	Point2Type projectPointCamCoords(const Point3Type &pnt) const; 
	
	/*Returns 3x4 camera projection matrix*/
	cv::Matx<FltType, 3, 4> cameraMat() const;

	/*Returns focal length along X axis (in pixels)*/
	FltType fx() const;
	
	/*Returns focal length along Y axis (in pixels)*/
	FltType fy() const;
	
	/*Returns X coordinate of the principal point*/
	FltType cx() const;
	
	/*Returns Y coordinate of the principal point*/
	FltType cy() const;
	
	/*Returns camera pose as cv::Affine3d*/
	cv::Affine3<FltType> pose() const;
	/*Camera translation*/
	VecType t;
	/*Camera rotation*/
	cv::Matx<FltType, 3, 3> R;
	/*Camera intrinsics
		(fx  0 cx)
		( 0 fy cy)
		( 0  0  1)
	*/
	cv::Matx<FltType, 3, 3> K;
	/*Radial distortion coefficients*/
	FltType k1, k2;
private:
	void distort(FltType &x, FltType &y) const;
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