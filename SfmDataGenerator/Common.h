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

#pragma once
#ifndef COMMON_H
#define COMMON_H
#include "stdafx.h"

#define SFMDATAGENERATOR_USE_DOUBLE

#ifdef SFMDATAGENERATOR_USE_DOUBLE
using FltType = double;
using _vtkArrayType = vtkDoubleArray;
#define TINYOBJLOADER_USE_DOUBLE
#define _CV_FLT CV_64F
#define _CV_FLT3 CV_64FC3
#define _VTK_FLT VTK_DOUBLE
#else
using FltType = float;
using _vtkArrayType = vtkFloatArray;
#define SFMDATAGENERATOR_USE_FLOAT
#define _CV_FLT CV_32F
#define _CV_FLT3 CV_32FC3
#define _VTK_FLT VTK_FLOAT
#endif

typedef cv::Vec<FltType, 3> VecType;
typedef cv::Point3_<FltType> Point3Type;
typedef cv::Point_<FltType> Point2Type;

#define btw(v, a, b) ((v >= a) && (v < b))
/*
	Class implementing a 3D projection onto a view.
	Observations can be compared
*/
class Observation {
public:
	/*Scene point descriptor*/
	int d;
	/*x coordinate of projection*/
	FltType x;
	/*y coordinate of projection*/
	FltType y;
	Observation(int d, FltType x, FltType y)
		: d(d), x(x), y(y) {}
	Observation()
		: Observation(0, 0.0, 0.0) {}
	Observation(int d, const cv::Point_<FltType> &p)
		: d(d), x(p.x), y(p.y) {}
	/*Makes observations comparable*/
	bool operator <(const Observation & obs) {
		return d < obs.d;
	}
};

typedef std::vector<Observation> View;

/*
	Computes point correspondences between two views
	complexity: O((|V1|+|V2|)*log(|V1|))
*/
static inline 
void getMatches(const View &view1, const View &view2, std::vector<cv::KeyPoint> &p1, std::vector<cv::KeyPoint> & p2) {
	std::map<int, cv::Point2f> obs1;
	for (const auto& obs : view1)
		obs1[obs.d] = cv::Point2f((float)obs.x, (float)obs.y);
	for (const auto& obs : view2) {
		auto it = obs1.find(obs.d);
		if (it == obs1.end())
			continue;
		p1.emplace_back(it -> second, 1.0f);
		p2.emplace_back((float)obs.x, (float)obs.y, 1.0f);
	}
}

/*String snippets*/

/*Converts string to lowercase*/
static inline 
std::string toLower(const std::string &s) {
	std::string res(s);
	for (char c : res)
		c = tolower(c);
	return res;
}

/*Extracts file extension*/
static inline 
std::string fileExt(const std::string &filename) {
	return std::filesystem::path(filename).extension().string();
}

static inline
std::vector<std::string> split(const std::string &s) {
	std::vector<std::string> res;
	std::string chunk;
	bool quote = false;
	for (char c : s) {
		if (c == '"') {
			quote = !quote;
			continue;
		}
		if (c == ' ') {
			if (quote)
				chunk += c;
			else {
				res.push_back(chunk);
				chunk = "";
			}
			continue;
		}
		chunk += c;
	}
	res.push_back(chunk);
	return res;
}

/*File and IO utils*/

/*
	Creates directory if it does'nt exist
	returns true if directory exists or if it was created successfully
	Implemented using C++17 experimental filesystem API
*/
static inline 
bool createDir(const std::string &path) {
	return std::experimental::filesystem::exists(path) ||
		std::experimental::filesystem::create_directories(path);
}

/*Reads Matx<T, m, n> from istream*/
template<class Tp, int m, int n>
static void readMatx(std::istream & is, cv::Matx<Tp, m, n> & mat) {
	for (int i = 0; i < n*m; ++i)
		is >> mat.val[i];
}

/*Writes Matx<T, m, n> to ostream*/
template<class Tp, int m, int n>
static void writeMatx(std::ostream & os, const cv::Matx<Tp, m, n> & mat) {
	for (int i = 0; i < n*m; ++i)
		os << mat.val[i] << ' ';
}

/*Two colors linear interpolation*/
static inline
cv::Vec3b interpolate(const cv::Vec3b & col1, const cv::Vec3b & col2, FltType value) {
	return (1.0 - value)*col1 + value*col2;
}

/*Maps value in the range [0; 1] to color in the range [blue; red]*/
static inline
cv::Vec3b valueToColor(FltType value) {
	if (value < -1.0) value = -1.0;
	if (value > 1.0) value = 1.0;
	if (value <= 0.25) {
		return interpolate({255, 0, 0}, {255, 255, 0}, 4.0*value);
	}
	if (value <= 0.5) {
		return interpolate({255, 255, 0}, {0, 255, 0}, 4.0*value - 1.0);
	}
	if (value <= 0.75) {
		return interpolate({0, 255, 0}, {0, 255, 255}, 4.0*value - 2.0);
	}
	return interpolate({0, 255, 255}, {0, 0, 255}, 4.0*value - 3.0);
}

static inline
void centralize(cv::InputOutputArray pointsArray, FltType cubeSize = -1.0) {
	cv::Mat points = pointsArray.getMat();
	if (points.type() != _CV_FLT3) return;
	VecType centroid(0, 0, 0);
	for (auto it = points.begin<VecType>(); it != points.end<VecType>(); ++it) {
		centroid += *it;
	}
	centroid *= 1.0 / points.total();
	for (auto it = points.begin<VecType>(); it != points.end<VecType>(); ++it) {
		*it -= centroid;
	}
	if (cubeSize > 0.0)
		cv::normalize(points, points, 0.5*cubeSize, 0.0, cv::NORM_INF);
}

static inline
cv::viz::Mesh loadMeshTypeEnforced(const std::string &filename, int type) {
	auto mesh = cv::viz::Mesh::load(filename, type);
	if (mesh.cloud.depth() != _CV_FLT) {
		mesh.cloud.convertTo(mesh.cloud, _CV_FLT);
		mesh.normals.convertTo(mesh.normals, _CV_FLT);
		mesh.tcoords.convertTo(mesh.tcoords, _CV_FLT);
	}
	return mesh;
}
#endif