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

enum Mode {
	SFM,
	SILHOUETTE,
	DEPTH
};

class GenHelper {
public:
	std::string imgFolder;
	SfmData &sfmData;
	cv::viz::Viz3d &viz;
	const cv::Mat &cloud;
	GenHelper(cv::viz::Viz3d &viz, const cv::Mat &cloud, SfmData &sfmData, const std::string& imgFolder, Mode mode);
	cv::Point3d getPoint(int idx) const;
	void changeCameraParams();
	void showCameraParams();
	void hideCameraParams();
	void takePhoto();
	void showProgress();
	void hideProgress();
private:
	void takeUsualPhoto();
	void takeSilhouette();
	void takeDepth();
	bool camParamsDisplay;
	bool progressDisplay;
	CameraCustomizer camParams;
	int counter;
	int n;
	Mode mode;
};
#endif
