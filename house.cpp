#include "house.h"

House::House(cv::Mat img, bool std) : MyObject(img) {
	this->feature = new HorizonLine(this);
	if (!std)
		this->stdFeature = HorizonLine::loadStdFeature(House::objName());
}

