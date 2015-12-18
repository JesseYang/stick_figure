#pragma once

#include "feature.h"
#include "my_object.h"

class DoubleCircle : public Feature {
public:
	DoubleCircle(MyObject *obj);
	DoubleCircle();
	void calculate();
	void plot();
	void updateStd();
	cv::Mat calRotMat(Feature *feature);
	cv::Rect getRoi(Feature *feature);

	double centerDist();
	static DoubleCircle *loadStdFeature(std::string objName);

	/// specific feature related data
	cv::Point center1;
	cv::Point center2;
	float radius1;
	float radius2;
};