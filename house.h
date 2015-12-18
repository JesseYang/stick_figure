#pragma once

#include "my_object.h"
#include "horizon_line.h"

class House : public MyObject {
public:
	House(cv::Mat img, bool std);

	std::string objName() { return "house"; };
};