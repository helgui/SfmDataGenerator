#pragma once
#ifndef MODEL_IMPORT_H
#define MODEL_IMPORT_H
#include "stdafx.h"
cv::Mat importObjModel(const std::string &filename, cv::viz::Viz3d &viz3d, bool returnCloud = true);
cv::Mat importPlyModel(const std::string &filename, cv::viz::Viz3d &viz3d, bool returnCloud = true);
cv::Mat importModel(const std::string &filename, cv::viz::Viz3d &viz3d, bool returnCloud);
#endif
