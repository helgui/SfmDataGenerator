#include "stdafx.h"

using namespace std;
using namespace cv;

void viewDataset(const string &inFile);
void genDataset(const string &inFile, const string &outFile);
void noisePoints(const string &inFile, const string &outFile, double stDev);
void addMatches(const string &inFile, const string & outFile, int count);

int focalX;
int focalY;
int K1;
int K2;

int main(int argc, char *argv[]) {
	String keys =
		"{@cmd           |help  | Command to execute (help, gen, view, noisp, falsm)}"
		"{stdev          |0.03  | Standart deviation                                }"
		"{in             |<none>| Input file                                        }"
		"{out            |<none>| Output file                                       }"
		"{count          |50    | Count of false matches                            }"
		;
	CommandLineParser parser(argc, argv, keys);
	parser.about("SfmDataGenerator v1.0.0");
	string cmd = parser.get<string>("@cmd");
	
	if (!parser.check()) {
		parser.printErrors();
		return 0;
	}

	if (cmd == "help") {
		parser.printMessage();
		return 0;
	}
	if (cmd == "view") {
		string inFile = parser.get<string>("in", false);
		if (!parser.check()) {
			parser.printErrors();
			return 0;
		}
		return 0;
	}
	if (cmd == "gen") {
		string inFile = parser.get<string>("in", false);
		string outFile = parser.get<string>("out", false);
		if (!parser.check()) {
			parser.printErrors();
			return 0;
		}
		genDataset(inFile, outFile);
		return 0;
	}
	if (cmd == "noisp") {
		string inFile = parser.get<string>("in", false);
		string outFile = parser.get<string>("out", false);
		double stdDev = parser.get<double>("stdev", true);
		if (!parser.check()) {
			parser.printErrors();
			return 0;
		}
		return 0;
	}
	if (cmd == "falsm") {
		string inFile = parser.get<string>("in", false);
		string outFile = parser.get<string>("out", false);
		int count = parser.get<int>("count", true);
		return 0;
	}
	return 0;
}

void genDataset(const string &inFile, const string &outFile) {
	const string winName = "Camera intrinsics";
	viz::Viz3d viz("virtual camera");
	viz.setBackgroundColor(viz::Color::white());
	viz::Mesh mesh = viz::readMesh(inFile);
	namedWindow("Camera intrinsics");
	resizeWindow(winName, 500, 170);
	createTrackbar("Focal X", winName, &focalX, 1024);
	createTrackbar("Focal Y", winName, &focalY, 1024);
	createTrackbar("k_1", winName, &K1, 100);
	createTrackbar("k_2", winName, &K2, 100);
	viz.showWidget("mesh", viz::WMesh(mesh));
	viz.spin();
}

