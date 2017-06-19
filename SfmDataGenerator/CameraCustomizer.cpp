/*
	Copyright (c) 2016 Oleg Yakovlev
	This file is a part of SfmDataGenerator software
*/
#include "stdafx.h"
#include "CameraCustomizer.h"
#include "Common.h"

using namespace std;
using namespace cv;

void CameraCustomizer::updateImage() {
	Vec3b topColor(2, 1, 1);
	Vec3b bottomColor(240, 120, 120);
	double delta = 1.0 / img.rows;
	double coeff = 0.0;
	for (int i = 0; i < img.rows; ++i, coeff += delta) {
		img.row(i).setTo(interpolate(topColor, bottomColor, coeff));
	}
	for (int i = 0; i < 2; ++i) {
		slider[i].draw(img(rect[i]));
	}

	Point2f offset(0.5f*img.cols, 0.75f*img.rows);

	for (int i = 1; i < (int)grid.size(); ++i) {
		for (int j = 1; j < (int)grid[i].size(); ++j) {
			const float& r1 = gridNorm[i][j];
			const float& r2 = gridNorm[i - 1][j];
			const float& r3 = gridNorm[i][j - 1];
			
			float m1 = 1.0f + k1()*r1*r1 + k2()*pow(r1, 4.0f);
			float m2 = 1.0f + k1()*r2*r2 + k2()*pow(r2, 4.0f);
			float m3 = 1.0f + k1()*r3*r3 + k2()*pow(r3, 4.0f);

			line(img, grid[i][j]*m1 + offset, grid[i-1][j]*m2 + offset, cv::Scalar(255, 255, 255));
			line(img, grid[i][j]*m1 + offset, grid[i][j-1]*m3 + offset, cv::Scalar(255, 255, 255));
		}
	}
	cv::putText(img, "K1:", cv::Point(10, SLIDER_HEIGHT / 2 + Slider::TRACK_HEIGHT / 2), cv::FONT_HERSHEY_PLAIN, 1.5, cv::Scalar(255, 255, 255), 2);
	cv::putText(img, "K2:", cv::Point(10, 3 * SLIDER_HEIGHT / 2 + Slider::TRACK_HEIGHT / 2), cv::FONT_HERSHEY_PLAIN, 1.5, cv::Scalar(255, 255, 255), 2);
}

CameraCustomizer::CameraCustomizer(const std::string &winName)
	: winName(winName), control(-1), changing(0), slider{
		{ -0.25, 0.25, 0, SLIDER_WIDTH, SLIDER_HEIGHT },
		{ -0.25, 0.25, 0, SLIDER_WIDTH, SLIDER_HEIGHT } },
	img(SLIDER_HEIGHT * 4 /*SLIDER_OFFSET + Slider::width(SLIDER_WIDTH)*/,
		SLIDER_OFFSET + Slider::width(SLIDER_WIDTH), CV_8UC3), grid() {
	for (int i = 0; i < 2; ++i) {
		rect[i].width = Slider::width(SLIDER_WIDTH);
		rect[i].height = SLIDER_HEIGHT;
		rect[i].x = SLIDER_OFFSET;
		rect[i].y = i*SLIDER_HEIGHT;
	}
	for (float y = -1.0f; y <= 1.0f; y += 0.2) {
		grid.emplace_back();
		gridNorm.emplace_back();
		for (float x = -1.0f; x <= 1.0f; x += 0.1) {
			grid.back().emplace_back(x*0.5f*img.cols, y*0.25f*img.rows);
			gridNorm.back().push_back(x*x + y*y);
		}
	}
	updateImage();
}

double CameraCustomizer::k1() const {
	return slider[0].value();
}

double CameraCustomizer::k2() const {
	return slider[1].value();
}