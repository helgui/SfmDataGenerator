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
#include "version.h"
#include "Common.h"
#include "SfmData.h"
#include "GenHelper.h"
#include "Slider.h"
#include "ModelImport.h"

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
	CONVERT,
	RUN_INTERACTIVE
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
	{ "noise-proj",			Command::NOISE_PTS},
	{ "c",					Command::CONVERT},
	{ "conv",				Command::CONVERT},
	{ "convert",			Command::CONVERT},
	{ "f",					Command::FALSE_OBS},
	{ "fo",					Command::FALSE_OBS},
	{ "false-proj",			Command::FALSE_OBS},
	{"run",					Command::RUN_INTERACTIVE},
	{ "r",					Command::RUN_INTERACTIVE}
};

SfmData curDataset;

void genDataset(SfmData &sfmData, const string &inFile, const string &outDir, Mode mode);

bool execute(const string &line) {
	auto v = split(line);
	const string &cmd = v[0];
	if (cmd == "e") return false;
	if (cmd == "v") {
		curDataset.show(cout);
		return true;
	}
	if (cmd == "vo") {
		curDataset.showObservations(stoi(v[1]), cout);
		return true;
	}
	if (cmd == "vm") {
		curDataset.showMatches(stoi(v[1]), stoi(v[2]), cout);
		return true;
	}
	if (cmd == "l") {
		curDataset.load(v[1]);
		return true;
	}
	if (cmd == "s") {
		curDataset.save(v[1]);
		return true;
	}
	if (cmd == "n") {
		curDataset.addGaussianNoise(stod(v[1]));
		return true;
	}
	if (cmd == "o") {
		curDataset.addOutliers(stoi(v[1]));
		return true;
	}
	if (cmd == "gd") {
		genDataset(curDataset, v[1], v[2], Mode::DEPTH);
		return true;
	}
	if (cmd == "g") {
		genDataset(curDataset, v[1], v[2], Mode::SFM);
		return true;
	}
	if (cmd == "gs") {
		genDataset(curDataset, v[1], v[2], Mode::SILHOUETTE);
		return true;
	}
	return false;
}

void runInteractive() {
	string s;
	do {
		getline(cin, s);
	} while (execute(s));
}

int main(int argc, char *argv[]) {
	vtkObject::GlobalWarningDisplayOff();
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
			curDataset.load(inFile);
			if (i2 != -1) {
				curDataset.showMatches(i1, i2, cout);
				return 0;
			}
			if (i1 != -1) {
				curDataset.showObservations(i1, cout);
				return 0;
			}
			curDataset.show(cout);
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
			genDataset(curDataset, inFile, outDir, mode);
			curDataset.save(outDir + "/" + defaultFilename + "." + format);
			return 0;
		}

		case Command::NOISE_PTS: {
			string inFile = parser.get<string>("in", false);
			string outFile = parser.get<string>("out", false);
			double stdDev = parser.get<FltType>("stdev", true);
			if (!parser.check()) {
				parser.printErrors();
				return 0;
			}
			curDataset.load(inFile);
			curDataset.addGaussianNoise(stdDev);
			curDataset.save(outFile);
			return 0;
		}

		case Command::FALSE_OBS: {
			string inFile = parser.get<string>("in", false);
			string outFile = parser.get<string>("out", false);
			if (!parser.check()) {
				parser.printErrors();
				return 0;
			}
			curDataset.load(inFile);
			if (parser.has("ratio")) {
				curDataset.addOutliers(parser.get<FltType>("ratio", true));
			}
			else {
				curDataset.addOutliers(parser.get<int>("count", true));
			}
			if (!parser.check()) {
				parser.printErrors();
				return 0;
			}
			curDataset.save(outFile);
			return 0;
		}

		case Command::CONVERT: {
			string inFile = parser.get<string>("in", false);
			string outFile = parser.get<string>("out", false);
			if (!parser.check()) {
				parser.printErrors();
				return 0;
			}
			curDataset.load(inFile);
			curDataset.save(outFile);
			return 0;
		}
		case Command::RUN_INTERACTIVE: {
			runInteractive();
			return 0;
		}
	}
	return 0;
}

void genDataset(SfmData &sfmData, const string &inFile, const string &outDir, Mode mode) {
	sfmData.clear();
	viz::Viz3d viz("Virtual camera");
	Mat cloud = importModel(inFile, viz, mode == Mode::SFM);
	viz::Camera cam = viz.getCamera();
	GenHelper helper(viz, cloud, sfmData, outDir + "/" + defaultImgFolder, mode);
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
	if (mode == Mode::SFM) {
		sfmData.fillCloud(cloud);
	}
	if (!sfmData.getViews().empty())
		sfmData.applyTransform(sfmData.getCameras()[0].pose());
}