/*
	Copyright (c) 2016 Oleg Yakovlev
	This file is a part of SfmDataGenerator software
*/
#ifndef SLIDER_H
#define SLIDER_H
#include "stdafx.h"
class Slider {
public:
	enum ConstParams {
		TRACK_HEIGHT = 10, 
		BUTTON_WIDTH = 16,
		TRACK_OFFSET = 2*BUTTON_WIDTH
	};
	static int width(int w);
	Slider(double min, double max, double val, int width, int height);
	void setValue(double newVal);
	void setPos(int pix);
	double value() const;
	double minValue() const;
	double maxValue() const;
	int width() const;
	int height() const;
	int trackWidth() const;
	void move(int pix);
	bool onTrack(const cv::Point &pnt) const;
	bool onButton(const cv::Point &pnt) const;
	void draw(cv::Mat &img, const cv::Scalar &buttonColor = cv::Scalar(160, 160, 160)) const;
private:
	double min, max, val;
	double scale;
	int w, h;
	cv::Rect track;
	cv::Point button;
	void updateButtonPos();
};
#endif
