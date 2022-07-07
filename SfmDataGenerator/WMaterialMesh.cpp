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
#include "WMaterialMesh.h"

using namespace std;
using namespace cv;
using namespace tinyobj;

void matToImageData(const Mat &from, vtkImageData *to) {
	Vec3b* dptr = reinterpret_cast<Vec3b*>(to->GetScalarPointer());
	size_t elem_step = to->GetIncrements()[1] / sizeof(Vec3b);

	for (int y = 0; y < from.rows; ++y) {
		Vec3b* drow = dptr + elem_step * y;
		const unsigned char *srow = from.ptr<unsigned char>(y);
		for (int x = 0; x < from.cols; ++x, srow += from.channels())
			drow[x] = Vec3b(srow[2], srow[1], srow[0]);
	}
}

WMaterialMesh::WMaterialMesh(vtkPolyData *polyData, const material_t &material) {
	vtkSmartPointer<vtkPolyDataMapper> mapper(vtkPolyDataMapper::New());
	mapper->SetInputData(polyData);
	vtkSmartPointer<vtkActor> actor(vtkActor::New());
	actor->SetMapper(mapper);
	auto prop = actor->GetProperty();
	prop->SetDiffuseColor(material.diffuse[0], material.diffuse[1], material.diffuse[2]);
	prop->SetAmbientColor(material.ambient[0], material.ambient[1], material.ambient[2]);
	prop->SetSpecularColor(material.specular[0], material.specular[1], material.specular[2]);
	prop->LightingOn();
	prop->ShadingOff();
	prop->SetInterpolationToPhong();
	prop->SetOpacity(material.dissolve);
	prop->SetSpecularPower(material.shininess);
	string texName;
	if (!material.diffuse_texname.empty()) {
		texName = material.diffuse_texname;
	}
	if (texName.empty() && !material.ambient_texname.empty()) {
		texName = material.ambient_texname;
	}
	if (!texName.empty()) {
		Mat img = imread(texName, IMREAD_COLOR);
		vtkSmartPointer<vtkImageData> imageData = vtkSmartPointer<vtkImageData>::New();
		vtkSmartPointer<vtkTexture> texture = vtkSmartPointer<vtkTexture>::New();
		imageData->SetDimensions(img.cols, img.rows, 1);
		imageData->AllocateScalars(VTK_UNSIGNED_CHAR, 3);
		imageData->SetSpacing(1.0, 1.0, 1.0);
		imageData->SetOrigin(0.0, 0.0, 0.0);
		matToImageData(img, imageData);
		imageData->Modified();
		texture->SetInputData(imageData);
		actor->SetTexture(texture);
	}
	viz::WidgetAccessor::setProp(*this, actor);
}

WMaterialMesh::WMaterialMesh(vtkPolyData *polyData) {
	vtkSmartPointer<vtkPolyDataMapper> mapper(vtkPolyDataMapper::New());
	mapper->SetInputData(polyData);
	vtkSmartPointer<vtkActor> actor(vtkActor::New());
	actor->SetMapper(mapper);
	auto prop = actor->GetProperty();
	prop->SetInterpolationToFlat();
	prop->BackfaceCullingOff();
	prop->ShadingOff();
	viz::WidgetAccessor::setProp(*this, actor);
}