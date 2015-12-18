#pragma once

#include "my_object.h"
#include "axis.h"

class Tree : public MyObject {
public:
	Tree(cv::Mat img, bool std);

	std::string objName() { return "tree"; };
};