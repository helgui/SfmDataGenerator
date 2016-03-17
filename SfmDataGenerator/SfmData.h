#ifndef SFM_DATA_H
#define SFM_DATA_H
#include "stdafx.h"
#include "Common.h"
#include "Camera.h"

class SfmData {
public:
	SfmData(const std::string &fileName);
	SfmData();
	bool load(const std::string &filename);
	bool save(const std::string &filename) const;
	void addView(const Camera &cam, const std::vector<Observation> &view, const std::string& imgFile);
	void fillCloud(const cv::Mat &cloud);
	void show() const;
	void showObservations(int viewIdx) const;
	void showMatches(int viewidx1, int viewIdx2) const;
	void addGaussianNoise(double stDev);
	void addFalseObservations(int count);
	void addFalseObservations(double ratio);
private:
	int maxIdx;
	std::vector <std::vector<Observation>> views;
	std::vector<Camera> cameras;
	std::vector<cv::Point3d> cloud;
	std::vector<std::string> images;
	bool loadFromTxt(const std::string &filename);
	bool loadFromXmlOrYml(const std::string &filename);
	bool saveToTxt(const std::string &filename) const;
	bool saveToXmlOrYml(const std::string &filename) const;
};
#endif
