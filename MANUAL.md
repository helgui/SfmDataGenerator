# Contents
 * [Usage](#usage)
 * [Output formats](#output-formats)

# Usage
```
SfmDataGenerator <command> -<param1>=<value1> -<param2>=<value2> ... <param_n>=<value_n>
```
Where `<command>` is one of:
 - [generate (gen, g)](#generate)
 - view (v)

In the following commands description `SfmDataGenerator` is omitted

# Commands
## generate
*Aliases: gen, g*

Run dataset generation

Usage:
```
gen -in=<input-ply> -out=<out-dir> -format=<fmt> -mode=<mode>
```
| Parameter     | Description                                      | Values        |
|---------------|--------------------------------------------------|---------------|
| `<input-ply>` | Path to input PLY model                          |               |
| `<out-dir>`   | Output directory                                 |               |
| `<fmt>`       | Format of dataset                                | txt, xml, yml |
| `<mode>`      | Dataset generation mode (SfM, Depth, Silhouette) | sfm, dep, sil |

# Output formats
## Output folder structure
### For SfM and Silhouette datasets
```
|--images
|  |--000001.png
|  |--000002.png
|  |-- ...
|--sfmData{.txt, .xml, .yml}
```
### For RGB-D Datasets
```
|--images
|  |--000001.exr (depth floating point image in OpenEXR)
|  |--000001.png (corresponding RGB image)
|  |--000002.exr
|  |--000002.png
|  |-- ...
|--sfmData{.txt, .xml, .yml}
```

## sfmData structure
### .txt
```
<nImages> <nPoints>
<path-to-image_1>                           (In case of RGB-D dataset this path references to a depth image)
<K_11> <K_12> <K_13> <K_21> ... <K_33>      (3x3 camera intrinsics matrix row by row)
<R_11> <R_12> <R_13> <R_21> ... <R_33>      (3x3 camera rotation matrix row by row)
<T_x> <T_y> <T_z>                           (camera translation vector)
<k_1> <k_2>                                 (Coefficients of radial distortion: k_1 for the second-order distortion and k_2 for the fourth-order distortion)
<nPointsOnImage_1>                            (Number of features)
<3d-point-index_1> <x_1> <y_1>              (Index of 3d point (from 0 to nPoints-1) and its projection)
<3d-point-index_2> <x_2> <y_2>
...
<3d-point-index_n> <x_n> <y_n>
<path-to-image_2>
...
<X_0> <Y_0> <Z_0>                           (0-indexed point cloud)
<X_1> <Y_1> <Z_1>
...
```
**Note: for silhouette & RGB-D datasets `<nPoints>`, `<nPointsOnImage>` are zeros**

### .yml
```yaml
%YAML:1.0
views:
    -
        image: <path-to-image_1>
        camera:
            - <K_11>
            - <K_12>
            ...
            - <K_33>
            - <R_11>
            - <R_12>
            ...
            - <R_33>
            - <T_x>
            - <T_y>
            - <T_z>
            - <k_1>
            - <k_2>
        observations:
            - <3d-point-index_1>
            - <x_1>
            - <y_1>
            - <3d-point-index_2>
            - <x_2>
            - <y_2>
            ...
    -
        image: <path-to-image_2>
        ...
    ...
points:
   - <X_0>
   - <Y_0>
   - <Z_0>
   - <X_1>
   - <Y_1>
   - <Z_1>
   ...
```
### .xml
``` xml
<?xml version="1.0"?>
<opencv_storage>
<views>
  <_>
    <image>path-to-image_1</image>
    <camera>K_11 K_12 ... R_11 ... R_33 ... k_1 k_2</camera>
    <observations>
      3d-point-index_1 x_1 y_1
      3d-point-index_2 x_2 y_2
      ...
    </observations>
  </_>
  ...
</views>
<points>
  X_0 Y_0 Z_0 X_1 Y_1 Z_1 ...
</points>
</opencv_storage>
```
### Reading .xml or .yml datasets with OpenCV (from SfmDataGenerator source)
``` C++
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
			double x, y;
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
		double x, y, z;
		(*it) >> x;
		++it;
		(*it) >> y;
		++it;
		(*it) >> z;
		cloud.emplace_back(x, y, z);
	}
}
```
