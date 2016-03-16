#include "stdafx.h"
#include "SfmData.h"
#include "version.h"

using namespace std;
using namespace cv;
using experimental::filesystem::create_directories;
using experimental::filesystem::exists;

void genDataset(const string &inFile, const string &outDir, const string &outFile);

const string defaultFilename = "sfmData";
const string defaultImgFolder = "images";

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
};

int main(int argc, char *argv[]) {
	String keys =
		"{@cmd           |help  | Command to execute (h, g, v, n, f)                  }"
		"{stdev          |0.03  | Standart deviation  (only for noise-points)         }"
		"{in             |<none>| Input file                                          }"
		"{out            |<none>| Output file or folder                               }"
		"{count          |50    | Count of false matches (only for false-observations)}"
		"{ratio          |0.2   | Ratio of false matches (only for false-obserbations)}"
		"{format         |txt   | Output file format (only for generate)              }"
		;
	CommandLineParser parser(argc, argv, keys);
	parser.about("SfmDataGenerator v" + to_string(SFM_DATA_GENERATOR_MAJ_VER) + "." +
		to_string(SFM_DATA_GENERATOR_MIN_VER));
	string cmd = parser.get<string>("@cmd", true);
	
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
		SfmData sfmData(inFile);
		sfmData.show();
		return 0;
	}
	if (cmd == "gen") {
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
	if (cmd == "noisp") {
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
		Camera cam(data->viz);
		vector<Observation> view;
		Size ws = data->viz.getWindowSize();
		for (int i = 0; i < (data->n); ++i) {
			Point3d pnt = data->cloud.at<Vec3f>(i);
			if (cam.toCameraCoords(pnt).z <= 0.0)
				continue;

			Point3d winCoords;
			data->viz.convertToWindowCoordinates(pnt, winCoords);
			
			//Frustum testing
			if (winCoords.x < 0 || winCoords.y < 0 || winCoords.x > ws.width || winCoords.y > ws.height) continue;
			
			//Depth testing
			if (winCoords.z > data->viz.getDepth(Point(winCoords.x, winCoords.y))) continue;
			view.emplace_back(i, cam.projectPoint(pnt));
		}
		ostringstream os;
		data->counter++;
		os << (data->imgFolder) << "/" << setw(6) << setfill('0') << data->counter  << ".png";
		data->viz.saveScreenshot(os.str());
		data->sfmData.addView(cam, view, os.str());
	}, &data);
	if (!exists(outDir) && !create_directories(outDir)) {
		cerr << "Can't create output folder\n";
		return;
	}
	if (!exists(outDir + "/" + defaultImgFolder) && !create_directories(outDir + "/" + defaultImgFolder)) {
		cerr << "Can't create output folder";
		return;
	}
	viz.spin();
	sfmData.fillCloud(mesh.cloud);
	sfmData.save(outFile);
}