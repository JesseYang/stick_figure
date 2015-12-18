#include "feature.h"
#include "my_object.h"

Feature::Feature(MyObject *obj) {
	this->obj = obj;
	this->width = obj->img.cols;
	this->height = obj->img.rows;
}

Feature::Feature()
{
}
