/*
	Copyright (c) 2016 Oleg Yakovlev
	This file is a part of SfmDataGenerator software
*/
#ifndef GEN_HELPER_H
#define GEN_HELPER_H
#include "stdafx.h"
#include "SfmData.h"
#include "Common.h"
#include "CameraCustomizer.h"

class GenHelper {
public:
	std::string imgFolder;
	SfmData &sfmData;
	cv::viz::Viz3d &viz;
	cv::Mat &cloud;
	GenHelper(cv::viz::Viz3d &viz, cv::Mat &cloud, SfmData &sfmData, const std::string& imgFolder);
	cv::Point3d getPoint(int idx) const;
	void changeCameraParams();
	void showCameraParams();
	void takePhoto();
private:
	CameraCustomizer camParams;
	int counter;
	int n;
};
#endif
