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
#include "ModelImport.h"
#include "Common.h"
#include "WMaterialMesh.h"
#define TINYOBJLOADER_IMPLEMENTATION
#include "tiny_obj_loader.h"

using namespace std;
using namespace cv;
using namespace viz;
using namespace tinyobj;

const bool FLIP_TCOORD_Y = true;

inline bool hasTexture(const material_t &mat) {
	return !mat.ambient_texname.empty() || !mat.diffuse_texname.empty();
}

Mat importObjModel(const std::string &filename, Viz3d &viz3d, bool returnCloud) {
	attrib_t attributes;
	vector<material_t> materials;
	vector<shape_t> shapes;
	ifstream objFile(filename);

	if (!objFile.is_open()) {
		cerr << "Can't open " + string(filename) << endl;
		return Mat();
	}

	string err;
	MaterialFileReader reader(".\\");
	bool ok = LoadObj(&attributes, &shapes, &materials, &err, &objFile, &reader);

	if (!err.empty()) {
		cerr << "TinyObj: got error " << err << endl;
	}

	if (!ok) {
		cerr << "TinyObj: can't parse " << filename << endl;
		return Mat();
	}

	Mat pointsProxy = Mat(attributes.vertices, false).reshape(3);
	centralize(pointsProxy);

	vector<vector<index_t>> faces(materials.size() + 1);
	vector<vector<int>> numFaceVertices(materials.size() + 1);
	for (const auto &shape : shapes) {
		size_t accum = 0;
		for (size_t f = 0; f < shape.mesh.num_face_vertices.size(); ++f) {
			auto fv = shape.mesh.num_face_vertices[f];
			int matIdx = shape.mesh.material_ids[f] > -1 ? shape.mesh.material_ids[f] : materials.size();
			numFaceVertices[matIdx].push_back(fv);
			for (size_t v = 0; v < fv; ++v) {
				faces[matIdx].push_back(shape.mesh.indices[accum + v]);
			}
			accum += fv;
		}
	}

	for (size_t i = 0; i < materials.size() + 1; ++i) {
		if (numFaceVertices[i].empty()) continue;
		vtkSmartPointer<vtkPolyData> polyData = vtkSmartPointer<vtkPolyData>::New();
		vtkSmartPointer<vtkPoints> points = vtkSmartPointer<vtkPoints>::New();
		vtkSmartPointer<vtkCellArray> polys = vtkSmartPointer<vtkCellArray>::New();
		vtkSmartPointer<vtkFloatArray> tcoords = vtkSmartPointer<vtkFloatArray>::New();
		tcoords->SetNumberOfComponents(2);
		vtkSmartPointer<vtkFloatArray> normals = vtkSmartPointer<vtkFloatArray>::New();
		normals->SetNumberOfComponents(3);
		size_t accum = 0;
		for (int fv : numFaceVertices[i]) {
			polys->InsertNextCell(fv);
			for (int v = 0; v < fv; ++v) {
				const index_t index = faces[i][accum + v];
				points->InsertNextPoint(attributes.vertices.data() + 3 * index.vertex_index);
				polys->InsertCellPoint(accum + v);
				if (index.texcoord_index > -1) {
					float u = attributes.texcoords[2 * index.texcoord_index];
					float v = attributes.texcoords[2 * index.texcoord_index + 1];
					if (FLIP_TCOORD_Y) v = 1.0f - v;
					tcoords->InsertNextTuple2(u, v);
				}
				if (index.normal_index > -1) {
					normals->InsertNextTuple(attributes.normals.data() + 3 * index.normal_index);
				}

			}
			accum += fv;
		}
		polyData->SetPoints(points);
		polyData->SetPolys(polys);
		if (points->GetNumberOfPoints() == tcoords->GetNumberOfTuples()) {
			if (i < materials.size() && hasTexture(materials[i]))
				polyData->GetPointData()->SetTCoords(tcoords);
		}
		if (points->GetNumberOfPoints() == normals->GetNumberOfTuples()) {
			polyData->GetPointData()->SetNormals(normals);
		}
		polyData->Squeeze();
		if (i < materials.size()) {
			viz3d.showWidget("mesh_" + to_string(i + 1), WMaterialMesh(polyData, materials[i]));
		} else {
			viz3d.showWidget("mesh_" + to_string(i + 1), WMaterialMesh(polyData));
		}
	}
	return returnCloud ? Mat(attributes.vertices, true).reshape(3) : Mat();
}

Mat importPlyModel(const std::string &filename, Viz3d &viz3d, bool returnCloud) {
	Mesh mesh = Mesh::load(filename, Mesh::LOAD_PLY);
	centralize(mesh.cloud);
	viz3d.showWidget("mesh_1", WMesh(mesh));
	return returnCloud ? mesh.cloud : Mat();
}

Mat importModel(const std::string &filename, Viz3d &viz3d, bool returnCloud) {
	if (fileExt(filename) == ".obj")
		return importObjModel(filename, viz3d, returnCloud);
	return importPlyModel(filename, viz3d, returnCloud);
}