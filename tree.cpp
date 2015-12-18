#include "tree.h"

Tree::Tree(cv::Mat img, bool std) : MyObject(img) {
	this->feature = new Axis(this);
	if (!std)
		this->stdFeature = Axis::loadStdFeature(Tree::objName());
}

