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
	void change();
	double k1() const;
	double k2() const;
private:
	enum ConstParams {
		SLIDER_WIDTH = 400,
		SLIDER_HEIGHT = 70,
		SLIDER_OFFSET = 50
	};
	void updateImage();
	cv::Mat img;
	Slider slider[2];
	cv::Rect rect[2];
	std::string winName;
	int control;
	bool changing;
};
#endif