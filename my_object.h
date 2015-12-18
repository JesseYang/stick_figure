#pragma once

#include <string>
#include "opencv2/imgproc/imgproc.hpp"
#include "fig_proc.h"
#include "common.h"
#include "feature.h"

class MyObject {
public:
	Feature *feature;
	Feature *stdFeature;
	cv::Mat oriImg;
	cv::Mat img;
	cv::Mat transImg;
	cv::Mat blurImg;
	cv::Mat erodeImg;
	std::vector<cv::Point> imgPts;

	MyObject(cv::Mat);
	virtual std::string objName() = 0;
	void calFeature();
	void plotFeature();
	void transform();
	void updateStdFeature();
	void evaluate();
};