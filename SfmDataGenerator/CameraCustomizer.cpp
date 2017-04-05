/*
	Copyright (c) 2016 Oleg Yakovlev
	This file is a part of SfmDataGenerator software
*/
#include "stdafx.h"
#include "CameraCustomizer.h"
#include "Common.h"

void CameraCustomizer::updateImage() {
	cv::Vec3b topColor(2, 1, 1);
	cv::Vec3b bottomColor(240, 120, 120);
	double delta = 1.0 / img.rows;
	double coeff = 0.0;
	for (int i = 0; i < img.rows; ++i, coeff += delta) {
		img.row(i).setTo(interpolate(topColor, bottomColor, coeff));
	}
	for (int i = 0; i < 2; ++i) {
		slider[i].draw(img(rect[i]));
	}
	cv::putText(img, "K1:", cv::Point(10, SLIDER_HEIGHT / 2 + Slider::TRACK_HEIGHT / 2), cv::FONT_HERSHEY_PLAIN, 1.5, cv::Scalar(255, 255, 255), 2);
	cv::putText(img, "K2:", cv::Point(10, 3 * SLIDER_HEIGHT / 2 + Slider::TRACK_HEIGHT / 2), cv::FONT_HERSHEY_PLAIN, 1.5, cv::Scalar(255, 255, 255), 2);
}

CameraCustomizer::CameraCustomizer(const std::string &winName)
	: winName(winName), control(-1), changing(0), slider{
		{ -1.0, 1.0, 0, SLIDER_WIDTH, SLIDER_HEIGHT },
		{ -1.0, 1.0, 0, SLIDER_WIDTH, SLIDER_HEIGHT } },
	img(SLIDER_HEIGHT * 2 /*SLIDER_OFFSET + Slider::width(SLIDER_WIDTH)*/,
		SLIDER_OFFSET + Slider::width(SLIDER_WIDTH), CV_8UC3) {
	for (int i = 0; i < 2; ++i) {
		rect[i].width = Slider::width(SLIDER_WIDTH);
		rect[i].height = SLIDER_HEIGHT;
		rect[i].x = SLIDER_OFFSET;
		rect[i].y = i*SLIDER_HEIGHT;
	}
	updateImage();
}

double CameraCustomizer::k1() const {
	return slider[0].value();
}

double CameraCustomizer::k2() const {
	return slider[1].value();
}