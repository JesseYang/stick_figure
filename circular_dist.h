#pragma once

#include "feature.h"

#include "my_object.h"

class CircularDist : public Feature {
public:
	CircularDist(MyObject *obj);
	CircularDist();
	void calculate();
	void plot();
	void updateStd();
	cv::Mat calRotMat(Feature *feature);
	cv::Rect getRoi(Feature *feature);

	double mainDir();
	static CircularDist *loadStdFeature(std::string objName);

	/// specific feature related data
	int binNum;
	std::vector<float> distAry;
	cv::Point center;
	float radius;
};