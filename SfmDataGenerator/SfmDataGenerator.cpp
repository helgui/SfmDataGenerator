#include "stdafx.h"
#include "SfmData.h"

using namespace std;
using namespace cv;

void viewDataset(const string &inFile);
void genDataset(const string &inFile, const string &outFile);

int main(int argc, char *argv[]) {
	String keys =
		"{@cmd           |help  | Command to execute (help, gen, view, noisp, falsm)}"
		"{stdev          |0.03  | Standart deviation                                }"
		"{in             |<none>| Input file                                        }"
		"{out            |<none>| Output file                                       }"
		"{count          |50    | Count of false matches                            }"
		"{ratio          |0.2   | Ratio of false matches                            }"
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
		SfmData sfmData(inFile);
		sfmData.addGaussianNoise(stdDev);
		sfmData.save(outFile);
		if (!parser.check()) {
			parser.printErrors();
			return 0;
		}
		return 0;
	}
	if (cmd == "falsm") {
		string inFile = parser.get<string>("in", false);
		string outFile = parser.get<string>("out", false);
		if (!parser.check()) {
			parser.printErrors();
			return 0;
		}
		SfmData sfmData(inFile);
		if (parser.has("ratio")) {
			sfmData.addFalseObservations(parser.get<double>("ratio", true));
		} else {
			sfmData.addFalseObservations(parser.get<int>("count", true));
		}
		if (!parser.check()) {
			parser.printErrors();
			return 0;
		}
		sfmData.save(outFile);
		return 0;
	}
	return 0;
}

void genDataset(const string &inFile, const string &outFile) {
	const string winName = "Camera intrinsics";
	viz::Viz3d viz("virtual camera");
	viz::Camera cam = viz.getCamera();
	viz.setBackgroundColor(viz::Color::white());
	viz::Mesh mesh = viz::readMesh(inFile);
}