#include "stdafx.h"
#include "WMaterialMesh.h"
#include "tiny_obj_loader.h"

using namespace std;
using namespace cv;
using namespace tinyobj;

void matToImageData(const Mat &from, vtkImageData *to) {
	Vec3b* dptr = reinterpret_cast<Vec3b*>(to->GetScalarPointer());
	size_t elem_step = to->GetIncrements()[1] / sizeof(Vec3b);

	for (int y = 0; y < from.rows; ++y)
	{
		Vec3b* drow = dptr + elem_step * y;
		const unsigned char *srow = from.ptr<unsigned char>(y);
		for (int x = 0; x < from.cols; ++x, srow += from.channels())
			drow[x] = Vec3b(srow[2], srow[1], srow[0]);
	}
}

WMaterialMesh::WMaterialMesh(vtkPolyData *polyData, const material_t &material) {
	vtkSmartPointer<vtkPolyDataMapper> mapper(vtkPolyDataMapper::New());
	mapper->ImmediateModeRenderingOff();
	mapper->SetInputData(polyData);
	vtkSmartPointer<vtkActor> actor(vtkActor::New());
	actor->SetMapper(mapper);
	auto prop = actor->GetProperty();
	prop->SetDiffuseColor(material.diffuse[0], material.diffuse[1], material.diffuse[2]);
	prop->SetAmbientColor(material.ambient[0], material.ambient[1], material.ambient[2]);
	prop->SetSpecularColor(material.specular[0], material.specular[1], material.specular[2]);
	prop->LightingOn();
	prop->SetInterpolationToFlat();
	prop->ShadingOff();
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
	mapper->ImmediateModeRenderingOff();
	mapper->SetInputData(polyData);
	vtkSmartPointer<vtkActor> actor(vtkActor::New());
	actor->SetMapper(mapper);
	auto prop = actor->GetProperty();
	prop->SetInterpolationToFlat();
	prop->BackfaceCullingOff();
	prop->ShadingOff();
	viz::WidgetAccessor::setProp(*this, actor);
}