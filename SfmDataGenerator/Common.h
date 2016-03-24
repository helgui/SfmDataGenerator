#pragma once
#ifndef COMMON_H
#define COMMON_H
#include "stdafx.h"

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
	double x;
	/*y coordinate of projection*/
	double y;
	Observation(int d, double x, double y)
		: d(d), x(x), y(y) {}
	Observation()
		: Observation(0, 0.0, 0.0) {}
	Observation(int d, const cv::Point2d &p)
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
	std::map<int, cv::Point2d> obs1;
	for (const auto& obs : view1)
		obs1[obs.d] = cv::Point2d(obs.x, obs.y);
	for (const auto& obs : view2) {
		auto it = obs1.find(obs.d);
		if (it == obs1.end())
			continue;
		//p1.emplace_back(it->second.x, it->second.y);
		//p2.emplace_back(obs.x, obs.y);
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
	for (int j = filename.size() - 1; j >= 0; --j) {
		if (filename[j] == '.') {
			return toLower(filename.substr(j, filename.size() - j));
		}
	}
	return std::string();
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

/*Two colors interpolation*/
static inline
cv::Vec3b interpolate(const cv::Vec3b & col1, const cv::Vec3b & col2, double value) {
	cv::Vec3d avg = (1.0 - value)*col1 + value*col2;
	return cv::Vec3b((uchar)avg[0], (uchar)avg[1], (uchar)avg[2]);
}

/*Maps value in the range [0; 1] to color in the range [blue; red]*/
static inline
cv::Vec3b valueToColor(double value) {
	if (value < 0.0 || value > 1.0)
		return cv::Vec3b(0, 0, 0);
	if (value <= 0.25) {
		return interpolate({255, 0, 0}, {255, 255, 0}, 4.0*value);
	}
	if (value <= 0.5) {
		return interpolate({ 255, 255, 0 }, {0, 255, 0}, 4.0*value - 1.0);
	}
	if (value <= 0.75) {
		return interpolate({ 0, 255, 0 }, { 0, 255, 255 }, 4.0*value - 2.0);
	}
	return interpolate({ 0, 255, 255 }, { 0, 0, 255 }, 4.0*value - 3.0);
}
#endif