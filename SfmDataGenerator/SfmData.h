#ifndef SFM_DATA_H
#define SFM_DATA_H
#include "stdafx.h"
class Observation {
public:
	int d;
	double x;
	double y;
	Observation(int d, double x, double y)
		: d(d), x(x), y(y) {}
	Observation() 
		: Observation(0, 0.0, 0.0) {}
};

typedef std::pair<double, double> Distortion;
class SfmData {
public:
	SfmData(const std::string &fileName);
	SfmData();
	bool load(const std::string &filename);
	bool save(const std::string &filename) const;
	void addView(const cv::Matx33d &intrinsic, const cv::Vec3d &rvec, const cv::Vec3d &tvec, const Distortion &dist, const std::vector<Observation> &view);
	void fillCloud(const cv::Mat &cloud);
	void show() const;
	void addGaussianNoise(double stDev);
	void addFalseObservations(int count);
	void addFalseObservations(double ratio);
private:
	int maxIdx;
	std::vector <std::vector<Observation>> views;
	std::vector<cv::Matx33d> intrinsics;
	std::vector<Distortion> dists;
	std::vector<cv::Vec3d> rvecs;
	std::vector<cv::Vec3d> tvecs;
	std::vector<cv::Point3d> cloud;
	bool loadFromTxt(const std::string &filename);
	bool loadFromXmlOrYml(const std::string &filename);
	bool saveToTxt(const std::string &filename) const;
	bool saveToXmlOrYml(const std::string &filename) const;
};
#endif
