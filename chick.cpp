#include "chick.h"

Chick::Chick(cv::Mat img, bool std) : MyObject(img) {
	this->feature = new DoubleCircle(this);
	if (!std)
		this->stdFeature = DoubleCircle::loadStdFeature(Chick::objName());
}
