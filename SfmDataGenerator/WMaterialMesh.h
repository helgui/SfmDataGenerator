#pragma once
#ifndef WMATERIAL_MESH_H
#define WMATERIAL_MESH_H
#include "stdafx.h"
#include "tiny_obj_loader.h"

class WMaterialMesh : public cv::viz::Widget3D {
public:
	WMaterialMesh(vtkPolyData *polyData, const tinyobj::material_t &mat);
	WMaterialMesh(vtkPolyData *polyData);
};
#endif
