#include "stdafx.h"
#include "Slider.h"
#include "Common.h"

Slider::Slider(double  min, double max, double value, int trackWidth, int sliderHeight)
	: min(min), max(max), w(trackWidth), h(sliderHeight),
	track(TRACK_OFFSET, (h - TRACK_HEIGHT) / 2, w, TRACK_HEIGHT),
	button(0, sliderHeight / 2){
	scale = (max - min) / w;
	setValue(value);
	updateButtonPos();
}

void Slider::updateButtonPos() {
	button.x = (int)(round((val - min) / scale) + 1e-3) + TRACK_OFFSET;
}

void Slider::draw(cv::Mat &img, const cv::Scalar &buttonColor) const {
	img(track).setTo(valueToColor((val - min) / (max - min)));
	cv::Rect rem(track);
	rem.x += (int)((val - min) / scale + 1e-3);
	rem.width -= rem.x - track.x;
	img(rem).setTo(cv::Vec3b(120, 120, 120));
	cv::rectangle(img, track, cv::Scalar(0, 0, 0));
	cv::circle(img, button, BUTTON_WIDTH / 2, buttonColor, -1);
	cv::circle(img, button, BUTTON_WIDTH / 2, cv::Scalar(0, 0, 0));
}

void Slider::setPos(int pix) {
	setValue(min + pix*scale);
}

bool Slider::onTrack(const cv::Point &pnt) const {
	return track.contains(pnt) && !onButton(pnt);
}

bool Slider::onButton(const cv::Point &pnt) const {
	return 2.0*cv::norm(pnt - button) <= BUTTON_WIDTH;
}

void Slider::setValue(double newVal) {
	if (newVal < min) {
		val = min;
		return;
	}
	if (newVal > max) {
		val = max;
		return;
	}
	val = newVal;
	updateButtonPos();
}

double Slider::value() const {
	return val;
}

double Slider::minValue() const {
	return min;
}

double Slider::maxValue() const {
	return max;
}

int Slider::trackWidth() const {
	return w;
}

int Slider::width() const {
	return width(w);
}

int Slider::height() const {
	return h;
}

void Slider::move(int pix) {
	setValue(val + pix*scale);
}

int Slider::width(int w) {
	return w + 4 * BUTTON_WIDTH;
}

