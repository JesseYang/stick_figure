#include "apple.h"

Apple::Apple(cv::Mat img, bool std) : MyObject(img) {
	this->feature = new CircularDist(this);
	if (!std)
		this->stdFeature = CircularDist::loadStdFeature(Apple::objName());
}

