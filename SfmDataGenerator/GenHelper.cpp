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
#include "GenHelper.h"

using namespace std;
using namespace cv;

GenHelper::GenHelper(viz::Viz3d &viz, const Mat &cloud, SfmData &sfmData, const string& imgFolder, Mode mode) :
	viz(viz), cloud(cloud), sfmData(sfmData), n(cloud.cols), counter(0), imgFolder(imgFolder), camParams("Lens distortion"), mode(mode),
	camParamsDisplay(0), progressDisplay(0) {
}

Point3d GenHelper::getPoint(int idx) const {
	return cloud.at<Vec3f>(idx);
}

void GenHelper::changeCameraParams() {
	showCameraParams();
	camParams.change([this]() -> void {
		this -> showCameraParams();
		auto ws = this->viz.getCamera().getWindowSize();
		auto p = this->viz.getCamera().getPrincipalPoint();
	});
	hideCameraParams();
}

void GenHelper::showCameraParams() {
	camParamsDisplay = 1;
	viz.showWidget("title", viz::WText("Camera parameters: ", Point(10, 120), 15, viz::Color::white()));
	viz.showWidget("focalx", viz::WText("Focal length (X): " + to_string(viz.getCamera().getFocalLength()[0]), Point(10, 100), 15, viz::Color::white()));
	viz.showWidget("focaly", viz::WText("Focal length (Y): " + to_string(viz.getCamera().getFocalLength()[1]), Point(10, 80), 15, viz::Color::white()));
	viz.showWidget("k1", viz::WText("Lens distrotion (K1): " + to_string(camParams.k1()), Point(10, 60), 15, viz::Color::white()));
	viz.showWidget("k2", viz::WText("Lens distortion (K2): " + to_string(camParams.k2()), Point(10, 40), 15, viz::Color::white()));
	auto ws = viz.getCamera().getWindowSize();
	viz.showWidget("imgsize", viz::WText("Image size: " + to_string(ws.width) + "x" + to_string(ws.height), Point(10, 20), 15, viz::Color::white()));
}

void GenHelper::hideCameraParams() {
	if (!camParamsDisplay) return;
	viz.removeWidget("title");
	viz.removeWidget("focalx");
	viz.removeWidget("focaly");
	viz.removeWidget("k1");
	viz.removeWidget("k2");
	viz.removeWidget("imgsize");
	camParamsDisplay = 0;
}

void GenHelper::takePhoto() {
	if (mode == Mode::SILHOUETTE) {
		takeSilhouette();
		return;
	}
	if (mode == Mode::DEPTH) {
		takeDepth();
		return;
	}
	takeUsualPhoto();
}

void GenHelper::takeUsualPhoto() {
	Size ws = viz.getCamera().getWindowSize();
	Camera cam(viz, camParams.k1(), camParams.k2());
	vector<Observation> view;
	Point3d winCoords;
	vtkRenderWindow *renderWindow = (vtkRenderWindow*)viz.getVtkRenderWindow();
	vtkSmartPointer<vtkFloatArray> zBufferRaw = vtkSmartPointer<vtkFloatArray>::New();
	zBufferRaw->SetNumberOfComponents(1);
	zBufferRaw->SetNumberOfValues(ws.area());
	renderWindow->GetZbufferData(0, 0, ws.width - 1, ws.height - 1, zBufferRaw);
	Mat depth(ws, CV_32F, zBufferRaw->GetPointer(0));
	for (int i = 0; i < n; ++i) {
		const Point3d& pnt = getPoint(i);
		const Point3d& pntCam = cam.toCameraCoords(pnt);
		const Point2d& pntImg = cam.projectPointCamCoords(pntCam);
		if (pntCam.z <= 0.0 || pntImg.x < 0.0 || pntImg.x > ws.width ||
			pntImg.y < 0.0 || pntImg.y > ws.height) {
			continue;
		}
		viz.convertToWindowCoordinates(pnt, winCoords);
		//Depth testing
		Point proj((int)round(winCoords.x), (int)round(winCoords.y));
		if (proj.x < 0) proj.x = 0;
		if (proj.x >= ws.width) proj.x = ws.width - 1;
		if (proj.y < 0) proj.y = 0;
		if (proj.y >= ws.height) proj.y = ws.height - 1;
		float d = depth.at<float>(proj);
		if (d != 1.0f && winCoords.z > d + 1e-3) continue;
		view.emplace_back(i, pntImg);
	}
	Mat img = viz.getScreenshot();
	ostringstream os;
	counter++;
	os << imgFolder << "/" << setw(6) << setfill('0') << counter << ".png";
	Mat undist;
	vector<Vec2f> dist;
	for (int i = 0; i < img.rows; ++i) {
		for (int j = 0; j < img.cols; ++j) {
			dist.emplace_back((float)j, (float)i);
		}
	}
	undistortPoints(dist, undist, cam.K, vector<double>{ cam.k1, cam.k2, 0.0, 0.0 }, noArray(), cam.K);
	undist = undist.reshape(0, img.rows);
	Mat newImg;
	remap(img, newImg, undist, noArray(), INTER_LINEAR, BORDER_REPLICATE);
	imwrite(os.str(), newImg);
	sfmData.addView(cam, view, os.str());
}

void GenHelper::takeSilhouette() {
	Size ws = viz.getCamera().getWindowSize();
	Camera cam(viz, 0.0, 0.0);
	vector<Observation> view;
	Mat screen(ws, CV_8UC3);
	vtkRenderWindow *renderWindow = (vtkRenderWindow*)viz.getVtkRenderWindow();
	vtkSmartPointer<vtkFloatArray> zBufferRaw = vtkSmartPointer<vtkFloatArray>::New();
	zBufferRaw->SetNumberOfComponents(1);
	zBufferRaw->SetNumberOfValues(ws.area());
	renderWindow->GetZbufferData(0, 0, ws.width - 1, ws.height - 1, zBufferRaw);
	Mat depth(ws, CV_32F, zBufferRaw->GetPointer(0));
	for (int i = 0; i < ws.height; ++i) {
		for (int j = 0; j < ws.width; ++j) {
			if (depth.at<float>(ws.height - i - 1, j) == 1.0f) {
				screen.at<Vec3b>(i, j) = Vec3b(0, 0, 0);
			}
			else {
				screen.at<Vec3b>(i, j) = Vec3b(255, 255, 255);
			}
		}
	}
	ostringstream os;
	counter++;
	os << imgFolder << "/" << setw(6) << setfill('0') << counter << ".png";
	imwrite(os.str(), screen);
	sfmData.addView(cam, view, os.str());
}

void GenHelper::showProgress() {
	progressDisplay = 1;
	viz.showWidget("progress", viz::WText("Processing...", Point(10, 20), 15, viz::Color::white()));
}

void GenHelper::hideProgress() {
	if (!progressDisplay) return;
	viz.removeWidget("progress");
}

void GenHelper::takeDepth() {
	Size ws = viz.getCamera().getWindowSize();
	Camera cam(viz, 0.0, 0.0);
	Vec2d clip = viz.getCamera().getClip();
	View view;
	vtkRenderWindow *renderWindow = (vtkRenderWindow*)viz.getVtkRenderWindow();
	vtkSmartPointer<vtkFloatArray> zBufferRaw = vtkSmartPointer<vtkFloatArray>::New();
	zBufferRaw->SetNumberOfComponents(1);
	zBufferRaw->SetNumberOfValues(ws.area());
	renderWindow->GetZbufferData(0, 0, ws.width - 1, ws.height - 1, zBufferRaw);
	Mat tmp(ws, CV_32F, zBufferRaw->GetPointer(0));
	Mat depth(ws, CV_32F);
	flip(tmp, depth, 0);
	for (int i = 0; i < ws.height; ++i) {
		for (int j = 0; j < ws.width; ++j) {
			float& d = depth.at<float>(i, j);
			if (d == 1.0f) continue;
			//convert OpenGL depth to a metric depth
			d = float((2.0 * clip[0] * clip[1]) / (clip[1] + clip[0] - (2.0*d - 1.0)*(clip[1] - clip[0])));
		}
	}
	ostringstream os;
	++counter;
	os << imgFolder << "/" << setw(6) << setfill('0') << counter;
	imwrite(os.str() + ".exr", depth);
	imwrite(os.str() + ".png", viz.getScreenshot()); //rgb image
	sfmData.addView(cam, view, os.str() + ".exr");
}