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
#include "SfmData.h"

using namespace std;
using namespace cv;

SfmData::SfmData()
	: views(), cloud(), cameras(), maxIdx(-1) {
}

SfmData::SfmData(const string &filename)
	: maxIdx(-1) {
	load(filename);
}

void SfmData::addView(const Camera &cam, const View &view, const string &imgFile) {
	for (const Observation& obs : view) {
		maxIdx = max(maxIdx, obs.d);
	}
	views.push_back(view);
	cameras.push_back(cam);
	images.push_back(std::experimental::filesystem::absolute(imgFile).string());
}

void SfmData::fillCloud(const Mat &mat) {
	
	vector<int> cnt(maxIdx + 1, 0);
	//Count observations of each point
	for (const auto& view : views) {
		for (const Observation& obs : view) {
			cnt[obs.d]++;
		}
	}

	for (int i = 0; i <= maxIdx; ++i) {
		if (cnt[i] < 2) {
			cnt[i] = -1;
		} else {
			cnt[i] = cloud.size();
			cloud.emplace_back(mat.at<VecType>(i)[0], mat.at<VecType>(i)[1], mat.at<VecType>(i)[2]);
		}
	}

	for (auto& view : views) {
		int cur = 0;
		for (int j = 0; j < (int)view.size(); ++j) {
			if (cnt[view[j].d] != -1) {
				view[j].d = cnt[view[j].d];
				view[cur++] = view[j];
			}
		}
		view.resize(cur);
	}

	//Very slow O(|Views|^2) empty views cleaning
	//I hope that there are O(1) empty views :) 
	for (int i = 0; i < (int)views.size(); ++i) {
		if (views[i].empty()) {
			views.erase(views.begin() + i);
			cameras.erase(cameras.begin() + i);
			images.erase(images.begin() + i);
			--i; //Elements after i-th idx are renumbered
		}
	}
	maxIdx = cloud.size() - 1;
}

bool SfmData::load(const string &filename) {
	string ext = fileExt(filename);
	if (ext == ".txt")
		return loadFromTxt(filename);
	if (ext == ".yml" || ext == ".xml")
		return loadFromXmlOrYml(filename);
	return false;
}

bool SfmData::save(const string &filename) const {
	string ext = fileExt(filename);
	if (ext == ".txt")
		return saveToTxt(filename);
	if (ext == ".yml" || ext == ".xml")
		return saveToXmlOrYml(filename);
	return false;
}

bool SfmData::saveToTxt(const string &filename) const {
	ofstream out(filename);
	out << setprecision(10);
	if (!out.is_open())
		return false;
	out << views.size() << ' ' << cloud.size() << endl;
	for (int i = 0; i < (int)views.size(); ++i) {
		out << images[i] << endl;
		writeMatx(out, cameras[i].K);
		out << endl;
		writeMatx(out, cameras[i].R);
		out << endl;
		writeMatx(out, cameras[i].t);
		out << endl;
		out << cameras[i].k1 << ' ' << cameras[i].k2 << endl;
		out << views[i].size() << endl;
		for (const Observation &obs : views[i]) {
			out << obs.d << ' ' << obs.x << ' ' << obs.y << endl;
		}
	}
	for (const Point3d &pnt : cloud) {
		out << pnt.x << ' ' << pnt.y << ' ' << pnt.z << endl;
	}
	return true;
}

void SfmData::applyTransform(const Affine3<FltType> &M) {
	auto M_inv = M.inv();
	for (auto& cam : cameras) {
		auto tmp = cam.pose()*M_inv;
		cam.R = tmp.rotation();
		cam.t = tmp.translation();
	}
	for (auto &p : cloud) {
		p = M*p;
	}
}

void SfmData::addGaussianNoise(FltType stDev) {
	if (stDev < 0.0) {
		for (auto& view : views) {
			for (auto& obs : view) {
				obs.x = round(obs.x);
				obs.y = round(obs.y);
			}
		}
	}
	normal_distribution<FltType> dist(0.0, stDev);
	mt19937 gen((unsigned)chrono::system_clock::now().time_since_epoch().count());
	for (auto& view : views) {
		for (auto& obs : view) {
			obs.x += dist(gen);
			obs.y += dist(gen);
		}
	}
}

void SfmData::showObservations(int viewIdx) const {
	if (!btw(viewIdx, 0, (int)views.size()))
		return;
	const string winName = "Image #" + to_string(viewIdx);
	Mat img = imread(images[viewIdx], IMREAD_ANYDEPTH | IMREAD_ANYCOLOR);
	Mat outImg;
	if (img.type() == CV_32F) { //depth image
		normalize(img, outImg, 0.0, 1.0, NORM_MINMAX);
	} else {
		vector <KeyPoint> kp;
		for (auto & obs : views[viewIdx]) {
			kp.emplace_back(Point2f((float)obs.x, (float)obs.y), 5.0f);
		}
		drawKeypoints(img, kp, outImg);
	}
	namedWindow(winName, 1);
	imshow(winName, outImg);
	waitKey();
	destroyWindow(winName);
}

void SfmData::showObservations(int viewIdx, ostream &os) const {
	showViewInfo(viewIdx, os);
	showObservations(viewIdx);
}

void SfmData::showMatches(int viewIdx1, int viewIdx2) const {
	if (!btw(viewIdx1, 0, (int)views.size()) || !btw(viewIdx2, 0, (int)views.size()))
		return;
	Mat img1 = imread(images[viewIdx1], IMREAD_ANYDEPTH | IMREAD_ANYCOLOR);
	Mat img2 = imread(images[viewIdx2], IMREAD_ANYDEPTH | IMREAD_ANYCOLOR);
	Mat out;
	if (img1.type() == CV_32F) {
		normalize(img1, img1, 0.0, 1.0, NORM_MINMAX);
		normalize(img2, img2, 0.0, 1.0, NORM_MINMAX);
		hconcat(img1, img2, out);
	} else {
		vector <KeyPoint> kp1, kp2;
		getMatches(views[viewIdx1], views[viewIdx2], kp1, kp2);
		vector <DMatch> matches;
		for (int i = 0; i < (int)kp1.size(); ++i)
			matches.emplace_back(i, i, 0.0f);
		drawMatches(img1, kp1, img2, kp2, matches, out);
	}
	namedWindow("Matches", 0);
	imshow("Matches", out);
	waitKey();
	destroyWindow("Matches");
}

void SfmData::showMatches(int viewIdx1, int viewIdx2, ostream &os) const {
	showMatchInfo(viewIdx1, viewIdx2, os);
	showMatches(viewIdx1, viewIdx2);
}

void SfmData::addOutliers(int count) {
	bool empty = true;
	for (const View &v : views) {
		empty = empty && views.empty();
	}
	if (empty || maxIdx <= 0) return;
	for (int i = 0; i < count; ++i) {
		int viewIdx = rand() % views.size();
		while (views[viewIdx].empty())
			viewIdx = rand() % views.size();
		int obsIdx = rand() % views[viewIdx].size();
		int newValue = rand() % (maxIdx + 1);
		while (views[viewIdx][obsIdx].d == newValue)
			newValue = rand() % (maxIdx + 1);
		views[viewIdx][obsIdx].d = newValue;
	}
}

void SfmData::addOutliers(FltType ratio) {
	int total = 0;
	for (const auto &view : views)
		total += view.size();
	int count = (int)(ratio*total);
	addOutliers(count);
}

void SfmData::show() const {
	viz::Viz3d viz("Point cloud");
	viz.setBackgroundColor(viz::Color::white());
	
	if (!cloud.empty()) 
		viz.showWidget("cloud", viz::WCloud(cloud, viz::Color::black()));
	
	for (int i = 0; i < (int)views.size(); ++i) {
		double sz = 1e10;
		if (views[i].empty()) sz = 1.0;
		for (const Observation& obs : views[i]) {
			auto pnt = cameras[i].toCameraCoords(cloud[obs.d]);
			sz = min(sz, fabs(pnt.z / 10.0));
		}
		viz.showWidget("cam" + to_string(i), viz::WCameraPosition(cameras[i].K, sz, viz::Color::red()), cameras[i].pose().inv());
	}
	viz.spin();
}

void SfmData::show(ostream &os) const {
	showInfo(os);
	show();
}

bool SfmData::loadFromTxt(const string &filename) {
	ifstream in(filename);
	if (!in.is_open())
		return false;
	clear();
	int viewCount, pointCount;
	in >> viewCount >> pointCount;
	for (int i = 0; i < viewCount; ++i) {
		string imgFile;
		in.ignore();
		getline(in, imgFile);
		vector<Observation> view;
		Camera cam;
		readMatx(in, cam.K);
		readMatx(in, cam.R);
		readMatx(in, cam.t);
		in >> cam.k1 >> cam.k2;
		int obsCount;
		int d;
		FltType x, y;
		in >> obsCount;
		for (int j = 0; j < obsCount; ++j) {
			in >> d >> x >> y;
			view.emplace_back(d, x, y);
		}
		addView(cam, view, imgFile);
	}
	for (int i = 0; i < pointCount; ++i) {
		FltType x, y, z;
		in >> x >> y >> z;
		cloud.emplace_back(x, y, z);
	}
	return true;
}

void SfmData::clear() {
	cloud.clear();
	views.clear();
	cameras.clear();
	images.clear();
	maxIdx = -1;
}

bool SfmData::loadFromXmlOrYml(const string &filename) {
	FileStorage fs(filename, FileStorage::READ);
	if (!fs.isOpened())
		return false;
	clear();
	for (auto &node : fs["views"]) {
		View view;
		Camera cam;
		string imgFile;
		node["image"] >> imgFile;
		node["camera"] >> cam;
		int it = 0;
		auto obsNode = node["observations"];
		for (auto it = obsNode.begin(); it != obsNode.end(); ++it) {
			int d;
			FltType x, y;
			(*it) >> d;
			++it;
			(*it) >> x;
			++it;
			(*it) >> y;
			view.emplace_back(d, x, y);
		}
		addView(cam, view, imgFile);
	}
	auto pointsNode = fs["points"];
	for (auto it = pointsNode.begin(); it != pointsNode.end(); ++it) {
		FltType x, y, z;
		(*it) >> x;
		++it;
		(*it) >> y;
		++it;
		(*it) >> z;
		cloud.emplace_back(x, y, z);
	}
	return true;
}

bool SfmData::saveToXmlOrYml(const string &filename) const {
	FileStorage fs(filename, FileStorage::WRITE);
	if (!fs.isOpened())
		return false;
	fs << "views" << "[";
	for (int i = 0; i < (int)views.size(); ++i) {
		fs << "{";
		fs << "image" << images[i];
		fs << "camera" << cameras[i];
		fs << "observations" << "[";
		for (const auto &obs : views[i])
			fs << obs.d << obs.x << obs.y;
		fs << "]";
		fs << "}";
	}
	fs << "]";
	fs << "points" << "[";
	for (const auto &pnt : cloud) {
		fs << pnt.x << pnt.y << pnt.z;
	}
	fs << "]";
	fs.release();
	return true;
}

void SfmData::showInfo(ostream & os) const {
	os << views.size() << " views" << endl;
	os << cloud.size() << " points in cloud" << endl;
}

void SfmData::showViewInfo(int idx, ostream & os) const {
	if (idx < 0 || idx >= (int)views.size()) {
		os << "Incorrect view index" << endl;
		return;
	}
	os << "View #" << idx << ' ' << views[idx].size() << " observations" << endl;
}

void SfmData::showMatchInfo(int idx1, int idx2, ostream & os) const {
	if (!btw(idx1, 0, (int)views.size()) || !btw(idx2, 0, (int)views.size())) {
		os << "Incorrect view index" << endl;
		return;
	}
	showViewInfo(idx1, os);
	showViewInfo(idx2, os);
	vector<KeyPoint> kp1;
	vector<KeyPoint> kp2;
	getMatches(views[idx1], views[idx2], kp1, kp2);
	os << kp1.size() << " matches" << endl;
}