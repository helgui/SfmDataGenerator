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
		for (int j = 0; j < img.cols; ++j) {
			img.at<cv::Vec3b>(i, j) = interpolate(topColor, bottomColor, coeff);
		}
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
	img(SLIDER_HEIGHT * 2, SLIDER_OFFSET + Slider::width(SLIDER_WIDTH), CV_8UC3) {
	for (int i = 0; i < 2; ++i) {
		rect[i].width = Slider::width(SLIDER_WIDTH);
		rect[i].height = SLIDER_HEIGHT;
		rect[i].x = SLIDER_OFFSET;
		rect[i].y = i*SLIDER_HEIGHT;
	}
	updateImage();
}

void CameraCustomizer::change() {
	if (changing)
		return;
	changing = 1;
	cv::namedWindow(winName);
	cv::imshow(winName, img);
	cv::setMouseCallback(winName, [](int event, int x, int y, int, void * cookie) -> void {
		CameraCustomizer *params = (CameraCustomizer *)cookie;
		if (event == cv::EVENT_LBUTTONDOWN) {
			int idx = -1;
			for (int i = 0; i < 2; ++i) {
				if (params->rect[i].contains(cv::Point(x, y))) {
					idx = i;
					break;
				}
			}
			if (idx == -1)
				return;
			cv::Point pnt = cv::Point(x, y) - (params->rect[idx].tl());
			if (params->slider[idx].onTrack(pnt)) {
				params->slider[idx].setPos(pnt.x - Slider::TRACK_OFFSET);
				params->updateImage();
				cv::imshow(params->winName, params->img);
				return;
			}
			if (params->slider[idx].onButton(pnt)) {
				params->control = idx;
			}
			return;
		}
		if (event == cv::EVENT_LBUTTONUP) {
			params->control = -1;
			return;
		}
		if (event == cv::EVENT_MOUSEMOVE) {
			if (params->control == -1)
				return;
			cv::Point pnt = cv::Point(x, y) - (params->rect[params->control].tl());
			params->slider[params->control].setPos(pnt.x - Slider::TRACK_OFFSET);
			params->updateImage();
			cv::imshow(params->winName, params->img);
		}
	}, this);
	cv::waitKey();
	cv::destroyWindow(winName);
	changing = 0;
}

double CameraCustomizer::k1() const {
	return slider[0].value();
}

double CameraCustomizer::k2() const {
	return slider[1].value();
}