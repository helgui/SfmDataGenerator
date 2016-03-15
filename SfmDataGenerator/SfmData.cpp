#include "stdafx.h"
#include "SfmData.h"

using namespace std;
using namespace cv;

inline string toLower(const string &s) {
	string res(s);
	for (char c : res)
		c = tolower(c);
	return res;
}

inline string fileExt(const string &filename) {
	for (int j = filename.size() - 1; j >= 0; --j) {
		if (filename[j] == '.') {
			return toLower(filename.substr(j, filename.size() - j));
		}
	}
	return string();
}

template<class Tp, int m, int n>
void readMatx(istream & is, cv::Matx<Tp, m, n> & mat) {
	for (int i = 0; i < n*m; ++i)
		is >> mat.val[i];
}

template<class Tp, int m, int n>
void writeMatx(ostream & os, const cv::Matx<Tp, m, n> & mat) {
	for (int i = 0; i < n*m; ++i)
		os << mat.val[i] << ' ';
}

SfmData::SfmData()
	: views(), cloud(), rvecs(), tvecs(), intrinsics(), dists(), maxIdx(-1) {
	
}

SfmData::SfmData(const string &filename) {
	load(filename);
}

void SfmData::addView(const cv::Matx33d &intrinsic, const cv::Vec3d &rvec, const cv::Vec3d &tvec, const Distortion &dist, const vector<Observation> &view) {
	for (const Observation& obs : view) {
		maxIdx = max(maxIdx, obs.d);
	}
	intrinsics.push_back(intrinsic);
	rvecs.push_back(rvec);
	tvecs.push_back(tvec);
	dists.push_back(dist);
}

void SfmData::fillCloud(const cv::Mat & mat) {
	vector<int> cnt(maxIdx + 1, 0);
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
			cloud.emplace_back(mat.at<cv::Vec3f>(i)[0], mat.at<cv::Vec3f>(i)[1], mat.at<cv::Vec3f>(i)[2]);
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
	if (!out.is_open())
		return false;
	out << views.size() << ' ' << cloud.size() << endl;
	for (int i = 0; i < (int)views.size(); ++i) {
		writeMatx(out, intrinsics[i]);
		out << endl;
		writeMatx(out, rvecs[i]);
		out << endl;
		writeMatx(out, tvecs[i]);
		out << endl;
		out << dists[i].first << ' ' << dists[i].second << endl;
		out << views[i].size() << endl;
		for (const Observation& obs : views[i]) {
			out << obs.d << ' ' << obs.x << ' ' << obs.y << endl;
		}
	}
	for (const cv::Point3d pnt : cloud) {
		out << pnt.x << ' ' << pnt.y << ' ' << pnt.z << endl;
	}
}

void SfmData::addGaussianNoise(double stDev) {
	if (stDev < 0.0) {
		for (auto& view : views) {
			for (auto& obs : view) {
				obs.x = round(obs.x);
				obs.y = round(obs.y);
			}
		}
	}
	normal_distribution<double> dist(0.0, stDev);
	mt19937 gen(chrono::system_clock::now().time_since_epoch().count());
	for (auto& view : views) {
		for (auto& obs : view) {
			obs.x += dist(gen);
			obs.y += dist(gen);
		}
	}
}

void SfmData::addFalseObservations(int count) {
	uniform_int_distribution<int> distViews(0, views.size() - 1);
	mt19937 gen(chrono::system_clock::now().time_since_epoch().count());
	for (int i = 0; i < count; ++i) {
		while (true) {
			int view1 = distViews(gen);
			int view2 = distViews(gen);
			if (view1 == view2)
				continue;
			uniform_int_distribution<int> distObs1(0, views[view1].size() - 1), distObs2(0, views[view2].size() - 1);
			int obs1 = distObs1(gen);
			int obs2 = distObs2(gen);
			if (views[view1][obs1].d == views[view2][obs2].d)
				continue;
			views[view1][obs1].d = views[view2][obs2].d;
			break;
		}
	}
}

void SfmData::addFalseObservations(double ratio) {
	int total = 0;
	for (const auto &view : views)
		total += view.size();
	int count = (ratio*total) / (1.0 - ratio);
	addFalseObservations(count);
}

void SfmData::show() const {
	viz::Viz3d viz("sfmData");
	viz.setBackgroundColor(viz::Color::white());
	Mat pts(maxIdx + 1, 1, CV_64FC3);
	for (int i = 0; i <= maxIdx; ++i)
		pts.at<Vec3d>(i) = cloud[i];
	viz.showWidget("cloud", viz::WCloud(pts, viz::Color::black()));
	for (int i = 0; i < (int)views.size(); ++i) {
		auto pose = Affine3d(rvecs[i], tvecs[i]);
		double sz = 1e10;
		for (const Observation& obs : views[i]) {
			auto pnt = pose*cloud[obs.d];
			sz = min(sz, fabs(pnt.z / 10.0));
		}
		viz.showWidget("cam" + to_string(i), viz::WCameraPosition(intrinsics[i], sz, viz::Color::red()), pose.inv());
	}
	viz.spin();
}

bool SfmData::loadFromTxt(const string &filename) {
	ifstream in(filename);
	if (!in.is_open())
		return false;
	int viewCount, pointCount;
	in >> viewCount >> pointCount;
	for (int i = 0; i < viewCount; ++i) {
		views.push_back(vector<Observation>());
		cv::Matx33d intrinsic;
		readMatx(in, intrinsic);
		cv::Vec3d rvec, tvec;
		readMatx(in, rvec);
		readMatx(in, tvec);
		Distortion dist;
		in >> dist.first >> dist.second;
		int obsCount;
		int d;
		double x, y;
		in >> obsCount;
		for (int j = 0; j < obsCount; ++j) {
			in >> d >> x >> y;
			views.back().emplace_back(d, x, y);
		}
	}
	for (int i = 0; i < pointCount; ++i) {
		double x, y, z;
		in >> x >> y >> z;
		cloud.emplace_back(x, y, z);
	}
	return true;
}

bool SfmData::loadFromXmlOrYml(const string &filename) {
	return true;
}

bool SfmData::saveToXmlOrYml(const string &filename) const {
	return true;
}