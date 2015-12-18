#pragma once

#include "feature.h"

#include "my_object.h"

class Axis : public Feature {
public:
	Axis(MyObject *obj);
	Axis();
	void calculate();
	void plot();
	void updateStd();
	cv::Mat calRotMat(Feature *feature);
	cv::Rect getRoi(Feature *feature);

	double axisLen();
	static Axis *loadStdFeature(std::string objName);

	/// specific feature related data
	cv::Point top;
	cv::Point bottom;
};