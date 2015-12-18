#pragma once

#include "my_object.h"
#include "double_circle.h"

class Bike : public MyObject {
public:
	Bike(cv::Mat img, bool std);

	std::string objName() { return "bike"; };
};