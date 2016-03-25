#include "stdafx.h"
#include "GenHelper.h"

using namespace std;
using namespace cv;

GenHelper::GenHelper(viz::Viz3d &viz, Mat &cloud, SfmData &sfmData, const string& imgFolder) :
	viz(viz), cloud(cloud), sfmData(sfmData), n(cloud.cols), counter(0), imgFolder(imgFolder), camParams("Lens distortion"){
	viz.showWidget("title", viz::WText("Camera parameters: ", Point(10, 120), 15, viz::Color::white()));
}

Point3d GenHelper::getPoint(int idx) const {
	return cloud.at<Vec3f>(idx);
}

void GenHelper::changeCameraParams() {
	camParams.change([this]() -> void {
		this -> showCameraParams();
		auto ws = this->viz.getCamera().getWindowSize();
		auto p = this->viz.getCamera().getPrincipalPoint();
	});
}

void GenHelper::showCameraParams() {
	viz.showWidget("focalx", viz::WText("Focal length (X): " + to_string(viz.getCamera().getFocalLength()[0]), Point(10, 100), 15, viz::Color::white()));
	viz.showWidget("focaly", viz::WText("Focal length (Y): " + to_string(viz.getCamera().getFocalLength()[1]), Point(10, 80), 15, viz::Color::white()));
	viz.showWidget("k1", viz::WText("Lens distrotion (K1): " + to_string(camParams.k1()), Point(10, 60), 15, viz::Color::white()));
	viz.showWidget("k2", viz::WText("Lens distortion (K2): " + to_string(camParams.k2()), Point(10, 40), 15, viz::Color::white()));
	auto ws = viz.getCamera().getWindowSize();
	viz.showWidget("imgsize", viz::WText("Image size: " + to_string(ws.width) + "x" + to_string(ws.height), Point(10, 20), 15, viz::Color::white()));
}

void GenHelper::takePhoto() {
	Size ws = viz.getCamera().getWindowSize();
	Camera cam(viz, camParams.k1(), camParams.k2());
	vector<Observation> view;
	for (int i = 0; i < n; ++i) {
		Point3d pnt = getPoint(i);
		if (cam.toCameraCoords(pnt).z <= 0.0)
			continue;

		Point3d winCoords;
		viz.convertToWindowCoordinates(pnt, winCoords);

		//Frustum testing
		if (winCoords.x < 0 || winCoords.y < 0 || winCoords.x > ws.width || winCoords.y > ws.height) continue;

		//Depth testing
		if (winCoords.z > viz.getDepth(Point((int)round(winCoords.x), (int)round(winCoords.y)))) continue;
		view.emplace_back(i, cam.projectPoint(pnt));
	}
	ostringstream os;
	counter++;
	os << imgFolder << "/" << setw(6) << setfill('0') << counter << ".png";
	imwrite(os.str(), viz.getScreenshot());
	sfmData.addView(cam, view, os.str());
}