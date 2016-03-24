# SfmDataGenerator
[Русская версия](README_RU.md)
## Overview
SfmDataGenerator is a software for generating realistic *Structure-from-Motion* (*SfM*) datasets. The dataset is being created by moving virtual camera around a 3D-model and taking photos from different viewpoints. A 3D-model is presented in [PLY](http://graphics.stanford.edu/data/3Dscanrep/#file_format) format. Virtual camera is fully consistent with the *pinhole camera* model.

SfmDataGenerator allows you to
 * Avoid resource-intensive feature detection and feature matching
 * Work with scenes of an arbitary geometric structure and with arbitary camera parameters
 * Test SfM software
 * Evaluate SfM software due to presence of *ground truth*
 * Evaluate SfM algorithms performance under noise

Generated dataset may be stored in *txt*, *yml* or *xml* files. Images are saved to *png* files.

SfmDataGenerator features:
 * Interactive SfM datasets generating
 * Adding gaussian noise and wrong matches
 * Dataset visualization:
   - Visualization of a point cloud and camera frustums
   - Visualization of points visible in one view
   - Visualization of point correspondences between two views
 * Different formats of dataset presentation

[Demo]()

## Pre-requisite
SfmDataGenerator is written using OpenCV library with modified Viz module, which you can build from [repository](https://github.com/helgui/opencv). If you already have OpenCV library intalled, you only need to rebuild Viz module.
## Use
See [manual](MANUAL.md)

PLY models are available at:
 * [The Stanford 3D Scanning Repository](http://graphics.stanford.edu/data/3Dscanrep)
 * [Large geometric models archive](http://www.cc.gatech.edu/projects/large_models)
 * [KSCAN3D Gallery](http://www.kscan3d.com/gallery)
 * [PLY Files an ASCII Polygon Format](http://people.sc.fsu.edu/~jburkardt/data/ply/ply.html)

##Author
Oleg Yakovlev

Contact the author at [Google group](https://groups.google.com/d/forum/sfmdatagen)
