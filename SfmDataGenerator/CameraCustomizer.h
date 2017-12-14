/*
MIT License

Copyright (c) 2017 Oleg Yakovlev

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#ifndef CAMERA_CUSTOMIZER_H
#define CAMERA_CUSTOMIZER_H
#include "stdafx.h"
#include "Slider.h"

class CameraCustomizer {
public:
	CameraCustomizer(const std::string &winName);
	template<class Callback>
	void change(Callback callback) {
		if (changing)
			return;
		changing = 1;
		cv::namedWindow(winName);
		cv::imshow(winName, img);
		struct Data {
			CameraCustomizer *camParams;
			Callback callback;
			Data(Callback cb, CameraCustomizer * cp)
				: callback(cb), camParams(cp) {
			}
 		};
		Data data(callback, this);
		cv::setMouseCallback(winName, [](int event, int x, int y, int, void * cookie) -> void {
			Data * data = (Data *)cookie;
			CameraCustomizer *params = data->camParams;
			Callback cb = data->callback;
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
					cb();
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
				cb();
			}
		}, &data);
		cv::waitKey();
		cv::destroyWindow(winName);
		changing = 0;
	}
	FltType k1() const;
	FltType k2() const;
private:
	enum ConstParams {
		SLIDER_WIDTH = 400,
		SLIDER_HEIGHT = 70,
		SLIDER_OFFSET = 50
	};
	//Using floats for grid points
	std::vector<std::vector<cv::Point2f>> grid;
	std::vector<std::vector<float>> gridNorm;
	void updateImage();
	cv::Mat img;
	Slider slider[2];
	cv::Rect rect[2];
	std::string winName;
	int control;
	bool changing;
};
#endif