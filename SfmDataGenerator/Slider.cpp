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

#include "stdafx.h"
#include "Slider.h"
#include "Common.h"

Slider::Slider(FltType  min, FltType max, FltType value, int trackWidth, int sliderHeight)
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

void Slider::setValue(FltType newVal) {
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

FltType Slider::value() const {
	return val;
}

FltType Slider::minValue() const {
	return min;
}

FltType Slider::maxValue() const {
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

