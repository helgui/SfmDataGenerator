/*
	Copyright (c) 2016 Oleg Yakovlev
	This file is a part of SfmDataGenerator software
*/
#include "stdafx.h"
#include "version.h"
#include "Common.h"
#include "SfmData.h"
#include "GenHelper.h"
#include "Slider.h"

using namespace std;
using namespace cv;
//Using experimental filesystem API
using experimental::filesystem::create_directories;
using experimental::filesystem::exists;

const string defaultFilename = "sfmData";
const string defaultImgFolder = "images";

const string keys =
"{@cmd           |help  | Command to execute    }"
"{stdev          |0.03  | Standart deviation    }"
"{in             |<none>| Input file            }"
"{out            |<none>| Output file or folder }"
"{count          |50    | Count of false matches}"
"{ratio          |0.2   | Ratio of false matches}"
"{format         |txt   | Output file format    }"
"{i1             |<none>| First view index      }"
"{i2             |<none>| Second view index     }"
"{mode           |sfm   | Mode (for `generate`) }";

//Supported commands
enum Command {
	GENERATE,
	VIEW,
	NOISE_PTS,
	FALSE_OBS,
	HELP,
	CONVERT
};

inline Mode modeFromString(const string &str) {
	auto lower = toLower(str);
	if (lower == "sil")
		return Mode::SILHOUETTE;
	if (lower == "dep")
		return Mode::DEPTH;
	return Mode::SFM;
}

//Command aliases
const map<string, Command> aliasOf = {
	{ "g",					Command::GENERATE},
	{ "gen",				Command::GENERATE},
	{ "generate",			Command::GENERATE},
	{ "v",					Command::VIEW},
	{ "view",				Command::VIEW},
	{ "h",					Command::HELP},
	{ "?",					Command::HELP},
	{ "help",				Command::HELP},
	{ "n",					Command::NOISE_PTS},
	{ "np",					Command::NOISE_PTS},
	{ "noise-points",		Command::NOISE_PTS},
	{ "c",					Command::CONVERT},
	{ "conv",				Command::CONVERT},
	{ "convert",			Command::CONVERT},
	{ "f",					Command::FALSE_OBS},
	{ "fo",					Command::FALSE_OBS},
	{ "false-observations",	Command::FALSE_OBS}
};

void genDataset(const string &inFile, const string &outDir, const string &outFile, Mode mode);

int main(int argc, char *argv[]) {
	CommandLineParser parser(argc, argv, keys);
	parser.about("SfmDataGenerator v" + to_string(SFM_DATA_GENERATOR_MAJ_VER) + "." +
		to_string(SFM_DATA_GENERATOR_MIN_VER));
	string cmd = toLower(parser.get<string>("@cmd", true));
	if (!parser.check()) {
		parser.printErrors();
		return 0;
	}
	auto it = aliasOf.find(cmd);
	if (it == aliasOf.end()) {
		cerr << "Unknown command \"" + cmd + "\"";
		return 0;
	}
	switch (it->second) {
		case Command::HELP: {
			parser.printMessage();
			return 0;
		}

		case Command::VIEW: {
			string inFile = parser.get<string>("in", false);
			int i1 = -1, i2 = -1;
			if (parser.has("i2")) {
				i1 = parser.get<int>("i1");
				i2 = parser.get<int>("i2");
			} else {
				if (parser.has("i1")) i1 = parser.get<int>("i1");
			}
			if (!parser.check()) {
				parser.printErrors();
				return 0;
			}
			SfmData sfmData(inFile);
			if (i2 != -1) {
				sfmData.showMatches(i1, i2, cout);
				return 0;
			}
			if (i1 != -1) {
				sfmData.showObservations(i1, cout);
				return 0;
			}
			sfmData.show(cout);
			return 0;
		}

		case Command::GENERATE: {
			string inFile = parser.get<string>("in", false);
			string outDir = parser.get<string>("out", false);
			string format = parser.get<string>("format", true);
			Mode mode = modeFromString(parser.get<string>("mode", true));
			if (format != "xml" && format != "yml" && format != "txt") {
				cerr << "Warning: Unknown format \"" + format + "\" replaced by default \"txt\" format";
				format = "txt";
			}
			if (!parser.check()) {
				parser.printErrors();
				return 0;
			}
			genDataset(inFile, outDir, outDir + "/" + defaultFilename + "." + format, mode);
			return 0;
		}

		case Command::NOISE_PTS: {
			string inFile = parser.get<string>("in", false);
			string outFile = parser.get<string>("out", false);
			double stdDev = parser.get<double>("stdev", true);
			if (!parser.check()) {
				parser.printErrors();
				return 0;
			}
			SfmData sfmData(inFile);
			sfmData.addGaussianNoise(stdDev);
			sfmData.save(outFile);
			return 0;
		}

		case Command::FALSE_OBS: {
			string inFile = parser.get<string>("in", false);
			string outFile = parser.get<string>("out", false);
			if (!parser.check()) {
				parser.printErrors();
				return 0;
			}
			SfmData sfmData(inFile);
			if (parser.has("ratio")) {
				sfmData.addFalseObservations(parser.get<double>("ratio", true));
			}
			else {
				sfmData.addFalseObservations(parser.get<int>("count", true));
			}
			if (!parser.check()) {
				parser.printErrors();
				return 0;
			}
			sfmData.save(outFile);
			return 0;
		}

		case Command::CONVERT: {
			string inFile = parser.get<string>("in", false);
			string outFile = parser.get<string>("out", false);
			if (!parser.check()) {
				parser.printErrors();
				return 0;
			}
			SfmData sfmData(inFile);
			sfmData.save(outFile);
			return 0;
		}
	}
	return 0;
}

void genDataset(const string &inFile, const string &outDir, const string & outFile, Mode mode) {
	SfmData sfmData;
	viz::Viz3d viz("Virtual camera");
	viz::Camera cam = viz.getCamera();
	viz::Mesh mesh = viz::readMesh(inFile);
	viz.showWidget("mesh", viz::WMesh(mesh));
	GenHelper helper(viz, mesh.cloud, sfmData, outDir + "/" + defaultImgFolder, mode);
	viz.registerKeyboardCallback([](const viz::KeyboardEvent &event, void * cookie) -> void {
		GenHelper * helper = (GenHelper *)cookie;
		if (event.action == viz::KeyboardEvent::KEY_DOWN) {
			if (event.symbol == "z") {
				helper->showCameraParams();
			}
			return;
		}

		if (event.symbol == "space") {
			helper->takePhoto();
			return;
		}
		if (event.symbol == "m") {
			helper->changeCameraParams();
			return;
		}
		if (event.symbol == "z") {
			helper->hideCameraParams();
			return;
		}
	}, &helper);

	if (!createDir(outDir)) {
		cerr << "Can't create output folder\n";
		return;
	}
	if (!createDir(outDir + "/" + defaultImgFolder)) {
		cerr << "Can't create output folder";
		return;
	}
	
	while (!viz.wasStopped()) {
		//helper.showCameraParams();
		viz.spinOnce(300, true);
	}
	if (mode == Mode::SFM)
		sfmData.fillCloud(mesh.cloud);
	sfmData.save(outFile);
}