#pragma once

#include "feature.h"

#include "my_object.h"

class HorizonLine : public Feature {
public:
	HorizonLine(MyObject *obj);
	HorizonLine();
	void calculate();
	void plot();
	void updateStd();
	cv::Mat calRotMat(Feature *feature);
	cv::Rect getRoi(Feature *feature);

	double lineLen();
	static HorizonLine *loadStdFeature(std::string objName);

	/// specific feature related data
	cv::Point left;
	cv::Point right;
};