#include "bike.h"

Bike::Bike(cv::Mat img, bool std) : MyObject(img) {
	this->feature = new DoubleCircle(this);
	if (!std)
		this->stdFeature = DoubleCircle::loadStdFeature(Bike::objName());
}
