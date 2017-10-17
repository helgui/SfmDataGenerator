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
