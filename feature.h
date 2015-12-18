#pragma once

#include <iostream>
#include <fstream>
#include "common.h"

class MyObject;

class Feature {
public:
	Feature(MyObject *obj);
	Feature();
	virtual void calculate() = 0;
	virtual void plot() = 0;
	virtual void updateStd() = 0;
	virtual cv::Mat calRotMat(Feature *feature) = 0;
	virtual cv::Rect getRoi(Feature *feature) = 0;
	int width;
	int height;
	cv::Mat transMat;
protected:
	MyObject *obj;
};
