#pragma once

#include "my_object.h"
#include "double_circle.h"

class Chick : public MyObject {
public:
	Chick(cv::Mat img, bool std);

	std::string objName() { return "chick"; };
};