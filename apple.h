#pragma once

#include "my_object.h"
#include "circular_dist.h"

class Apple : public MyObject {
public:
	Apple(cv::Mat img, bool std);

	std::string objName() { return "apple"; };
};