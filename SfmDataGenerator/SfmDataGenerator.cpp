#include "stdafx.h"
#include "Common.h"
#include "SfmData.h"
#include "version.h"

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
"{i2             |<none>| Second view index     }";

//Supported commands
enum Command {
	GENERATE,
	VIEW,
	NOISE_PTS,
	FALSE_OBS,
	HELP,
	CONVERT
};

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

//Data for Viz3d keyboard callback
class CBData {
public:
	int n;
	int counter;
	string imgFolder;
	SfmData &sfmData;
	viz::Viz3d &viz;
	cv::Mat &cloud;
	CBData(viz::Viz3d &viz, cv::Mat &cloud, SfmData &sfmData, const string& imgFolder) :
		viz(viz), cloud(cloud), sfmData(sfmData), n(cloud.cols), counter(0), imgFolder(imgFolder){}
	Point3d getPoint(int idx) {
		return cloud.at<Vec3f>(idx);
	}
};

void genDataset(const string &inFile, const string &outDir, const string &outFile);

int main(int argc, char *argv[]) {
	
	CommandLineParser parser(argc, argv, keys);
	parser.about("SfmDataGenerator v" + to_string(SFM_DATA_GENERATOR_MAJ_VER) + "." +
		to_string(SFM_DATA_GENERATOR_MIN_VER));
	string cmd = parser.get<string>("@cmd", true);
	
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
			}
			else {
				if (parser.has("i1")) i1 = parser.get<int>("i1");
			}
			if (!parser.check()) {
				parser.printErrors();
				return 0;
			}
			SfmData sfmData(inFile);
			if (i2 != -1) {
				sfmData.showMatches(i1, i2);
				return 0;
			}
			if (i1 != -1) {
				sfmData.showObservations(i1);
				return 0;
			}
			sfmData.show();
			return 0;
		}
		case Command::GENERATE: {
			string inFile = parser.get<string>("in", false);
			string outDir = parser.get<string>("out", false);
			string format = parser.get<string>("format", true);
			if (format != "xml" && format != "yml" && format != "txt") {
				cerr << "Warning: Unknown format \"" + format + "\" replaced by default \"txt\" format";
				format = "txt";
			}
			if (!parser.check()) {
				parser.printErrors();
				return 0;
			}
			genDataset(inFile, outDir, outDir + "/" + defaultFilename + "." + format);
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

void genDataset(const string &inFile, const string &outDir, const string & outFile) {
	SfmData sfmData;
	const string winName = "Camera intrinsics";
	viz::Viz3d viz("virtual camera");
	viz::Camera cam = viz.getCamera();
	viz.setBackgroundColor(viz::Color::white());
	viz::Mesh mesh = viz::readMesh(inFile);
	viz.showWidget("mesh", viz::WMesh(mesh));
	CBData data(viz, mesh.cloud, sfmData, outDir + "/" + defaultImgFolder);
	viz.registerKeyboardCallback([](const viz::KeyboardEvent &event, void * cookie) -> void {
		if (event.action != viz::KeyboardEvent::KEY_UP || event.symbol != "space")
			return;
		CBData * data = (CBData *)cookie;
		Size ws = data->viz.getWindowSize();
		Camera cam(data->viz);
		vector<Observation> view;
		for (int i = 0; i < (data->n); ++i) {
			Point3d pnt = data->getPoint(i);
			if (cam.toCameraCoords(pnt).z <= 0.0)
				continue;

			Point3d winCoords;
			data->viz.convertToWindowCoordinates(pnt, winCoords);
			
			//Frustum testing
			if (winCoords.x < 0 || winCoords.y < 0 || winCoords.x > ws.width || winCoords.y > ws.height) continue;
			
			//Depth testing
			if (winCoords.z > data->viz.getDepth(Point((int)round(winCoords.x), (int)round(winCoords.y)))) continue;
			view.emplace_back(i, cam.projectPoint(pnt));
		}
		ostringstream os;
		data->counter++;
		os << (data->imgFolder) << "/" << setw(6) << setfill('0') << data->counter  << ".png";
		data->viz.saveScreenshot(os.str());
		data->sfmData.addView(cam, view, os.str());
	}, &data);
	if (!createDir(outDir)) {
		cerr << "Can't create output folder\n";
		return;
	}
	if (!createDir(outDir + "/" + defaultImgFolder)) {
		cerr << "Can't create output folder";
		return;
	}
	viz.spin();
	sfmData.fillCloud(mesh.cloud);
	sfmData.save(outFile);
}