/*
	Copyright (c) 2016 Oleg Yakovlev
	This file is a part of SfmDataGenerator software
*/
#ifndef SFM_DATA_H
#define SFM_DATA_H
#include "stdafx.h"
#include "Common.h"
#include "Camera.h"

class SfmData {
public:
	/*Loads SfmData from file*/
	SfmData(const std::string &fileName);
	
	/*Creates an empty SfmData instance*/
	SfmData();
	/*
	Loads sfmData from file. Determines format
	automatically
	*/
	bool load(const std::string &filename);
	/*
	Saves sfmData to file. Determines file format
	automatically
	*/
	bool save(const std::string &filename) const;
	/*Adds a view with given obsevations, camera and image file*/
	void addView(const Camera &cam, const View &view, const std::string& imgFile);
	/*
	Fills internal cloud from cloud given as cv::Mat
	It's necessary to be called after SfmData building and before saving or
	other usage, because it makes SfmData consistent i.e. it
		1. erases points observing from only one view
		2. erases empty views
		3. compresses points descriptors, thereby making them strickly consequtive
		4. finally fills SfmData::cloud
	*/
	void fillCloud(const cv::Mat &cloud);
	/*Shows sfmData in 3D as cloud and camera frustums with*/
	void show() const;
	void show(std::ostream &os) const;

	/*Shows observations of a given view with*/
	void showObservations(int viewIdx) const;
	void showObservations(int viewIdx, std::ostream &os) const;
	/*Shows point correspondences between two views*/
	void showMatches(int viewIdx1, int viewIdx2) const;
	void showMatches(int viewidx1, int viewIdx2, std::ostream &os) const;
	/*
	Adds a Gaussian noise with given standart deviation
	to coordinates of projection or
	rounds its values to integer if stDev < 0
	*/
	void addGaussianNoise(double stDev);
	/*Adds a given number of false observations*/
	void addFalseObservations(int count);
	/*
	Adds false observations to SfmData by given
	ratio
	*/
	void addFalseObservations(double ratio);
	/*Clean up all data*/
	void clear();
	void showInfo(std::ostream & os = std::cout) const;
	void showViewInfo(int idx1, std::ostream & os = std::cout) const;
	void showMatchInfo(int idx1, int idx2, std::ostream & os = std::cout) const;
private:
	/*Maximum point descriptor*/
	int maxIdx;
	/*Observations for each view*/
	std::vector <View> views;
	/*Camera for each view*/
	std::vector<Camera> cameras;
	/*3D points*/
	std::vector<cv::Point3d> cloud;
	/*Image file names*/
	std::vector<std::string> images;
	/*loads SfmData from txt file*/
	bool loadFromTxt(const std::string &filename);
	/*Loads SfmData from OpenCV File storage*/
	bool loadFromXmlOrYml(const std::string &filename);
	/*Saves SfmData to txt file*/
	bool saveToTxt(const std::string &filename) const;
	/*Saves sfmData to OpenCV File storage*/
	bool saveToXmlOrYml(const std::string &filename) const;
};
#endif
