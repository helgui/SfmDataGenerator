# Contents
 * [Usage](#usage)
 * [Commands](#commands)
 * [Controls](#controls)
 * [Output formats](#output-formats)

# Usage
```
SfmDataGenerator <command> -<param1>=<value1> -<param2>=<value2> ... <param_n>=<value_n>
```
Where `<command>` is one of:
 - [help, (h, ?)](#help)
 - [generate (gen, g)](#generate)
 - [view (v)](#view)
 - [convert (conv, c)](#convert)
 - [noise-proj (np, n)](#noise-proj)
 - [false-proj (fp, f)](#false-proj)

In the following commands description `SfmDataGenerator` is omitted

# Commands
## help
*Aliases: h, ?*

Show help

Usage:
```
help
```
## generate
*Aliases: gen, g*

Run dataset generation

Usage:
```
gen -in=<model> -tex=<texture> -out=<out-dir> [-format=<fmt>] [-mode=<mode>]
```

| Parameter     | Description                                      | Values        | Default |
|---------------|--------------------------------------------------|---------------|---------|
| `<model>`| Path to input model                              |               |		 |
| `<out-dir>`  | Output directory                                 |               |			 |
| `<fmt>`       | Format of dataset                                | txt, xml, yml | txt	 |
| `<mode>`      | Dataset generation mode (SfM, Depth, Silhouette) | sfm, dep, sil | sfm     |
| `<texture>`   | Path to texture file (only for OBJ-models)       |               |        |

## view
*Alias: v*

View generated dataset 

Usage:
```
view -in=<sfm-data> [-i1=<first-idx>] [-i2=<second-idx>]
```

| Parameter    | Description          |
|--------------|----------------------|
| `<sfm-data>`   | Path to sfmData file |
| `<first-idx>`  | First image index    |
| `<second-idx>` | Second image index   |

Effect of `view` depends on parameters passed as well as type of dataset. 

| Dataset type | i1 | i2 | Effect                                        |
|--------------|----|----|-----------------------------------------------|
| SfM          | -  | -  | View point cloud & camera poses               |
| SfM          | +  | -  | View features of (i1)-th image                |
| SfM          | +  | +  | View matches between (i1)-th & (i2)-th images |
| RGB-D & Sil  | -  | -  | View camera poses                             |
| RGB-D & Sil  | +  | -  | View (i1)-th depth (or silhouette) image      |

## convert
*Aliases: conv, c*

Convert dataset from one format to another. It uses file extension to determine format.

Usage:
```
conv -in=<input> -out=<output>
``` 

| Parameter | Description         |
|-----------|---------------------|
| `<input>`   | Input sfmData file  |
| `<output>`  | Output sfmData file |

## noise-proj
**Note: only for SfM datasets**

*Aliases: np, n*

Add gaussian noise to each projection coordinates

Usage:
```
np -in=<input> -out=<output> [-stdev=<stdev>]
```

| Parameter | Description                                       | Default |
|-----------|---------------------------------------------------|---------|
| `<input>`   | Input sfmData file                                |         |
| `<output>`  | File to save noisy dataset                        |         |
| `<stdev>`   | Strandard deviation of gaussian noise (in pixels) | 0.03    |

## false-proj
**Note: only for SfM datasets**

*Aliases: fp, f*

Add wrong point-projection correspondences. It emulates a real feature matching.

Usage:
```
fp -in=<input> -out=<output> [-ratio=<ratio>] [-count=<count>]
```

| Parameter | Description                     | Default |
|-----------|---------------------------------|---------|
| `<input>`   | Input sfmData file              |         |
| `<output>`  | File to save noisy dataset      |         |
| `<ratio>`   | Ratio of wrong correspondences  | 0.2     |
| `<count>`   | Number of wrong correspondences | 50      |

If `<ratio>` and `<count>` are both given, `<ratio>` has priority.

#Controls

			 
# Output formats
## Output folder structure
### For SfM and Silhouette datasets
```
output_dir
|
|--images
|  |--000001.png
|  |--000002.png
|  |-- ...
|--sfmData{.txt, .xml, .yml}
```
### For RGB-D Datasets
```
output_dir
|
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
**Note: for silhouette & RGB-D datasets `<nPoints>`, `<nPointsOnImage>` are zeros, i.e. only camera intrinsics & extrinsics are presented**

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
