#include "axis.h"

Axis::Axis(MyObject *obj) : Feature(obj) {
}

Axis::Axis() : Feature() {
}

void Axis::calculate() {
	std::vector<cv::Point> hull;
	cv::convexHull((cv::Mat)this->obj->imgPts, hull, false);

	int cur_d = 0, d = 0;
	cv::Point p1, p2;
	for (std::vector<cv::Point>::iterator outer = hull.begin(); outer < hull.end(); ++outer) {
		for (std::vector<cv::Point>::iterator inner = hull.begin(); inner < hull.end(); ++inner) {
			cur_d = pow((*outer).x - (*inner).x, 2) + pow((*outer).y - (*inner).y, 2);
			if (cur_d > d) {
				p1 = (*outer);
				p2 = (*inner);
				d = cur_d;
			}
		}
	}
	
	int d1 = 0, d2 = 0, nearP1 = 0, nearP2 = 0;
	for (std::vector<cv::Point>::iterator it = this->obj->imgPts.begin(); it < this->obj->imgPts.end(); ++it) {
		d1 = pow((*it).x - p1.x, 2) + pow((*it).y - p1.y, 2);
		d2 = pow((*it).x - p2.x, 2) + pow((*it).y - p2.y, 2);
		if (d1 < d2) {
			nearP1++;
		}
		else {
			nearP2++;
		}
	}
	if (nearP1 < nearP2) {
		this->top = p1;
		this->bottom = p2;
	}
	else {
		this->top = p2;
		this->bottom = p1;
	}
}

void Axis::plot() {
	cv::Mat colorTransImg;
	cv::cvtColor(this->obj->transImg, colorTransImg, cv::COLOR_GRAY2BGR);
	fig_proc::plotArrowedLine(colorTransImg, this->bottom, this->top);
}

void Axis::updateStd() {
	std::ofstream file(file_proc::getPathByName(this->obj->objName(), file_proc::STD_FEATURE));

	file << "size:" << this->width << "," << this->height << std::endl;
	file << "top:" << this->top.x << "," << this->top.y << std::endl;
	file << "bottom:" << this->bottom.x << "," << this->bottom.y << std::endl;
	file.close();
}

cv::Mat Axis::calRotMat(Feature *feature)
{
	Axis *axis = (Axis *)feature;
	double scale = axis->axisLen() / this->axisLen();
	cv::Point center(this->top.x + this->bottom.x / 2, this->top.y + this->bottom.y / 2);
	double angle1 = fig_proc::findAngle(axis->bottom, axis->top);
	double angle2 = fig_proc::findAngle(this->bottom, this->top);
	double angle = angle2 - angle1;
	this->transMat = cv::getRotationMatrix2D(center, angle, scale);
	return this->transMat;
}

cv::Rect Axis::getRoi(Feature *feature)
{
	Axis *axis = (Axis *)feature;
	std::vector<cv::Point> v1;
	v1.push_back(cv::Point((this->top.x + this->bottom.x) / 2, (this->top.y + this->bottom.y) / 2));
	std::vector<cv::Point> v2;
	cv::Point transCenter;
	cv::transform(v1, v2, this->transMat);
	int xStart = v2[0].x - (axis->top.x + axis->bottom.x) / 2;
	int yStart = v2[0].y - (axis->top.y + axis->bottom.y) / 2;
	cv::Rect roi(xStart, yStart, feature->width, feature->height);
	return roi;
}

double Axis::axisLen()
{
	return sqrt(pow(this->top.x - this->bottom.x, 2) + pow(this->top.y - this->bottom.y, 2));
}

Axis *Axis::loadStdFeature(std::string objName) {
	std::ifstream file(file_proc::getPathByName(objName, file_proc::STD_FEATURE));
	std::string line;
	std::vector<std::string> lineContent;
	std::vector<std::string> content;
	// Todo: read data from file
	Axis *axis = new Axis();
	while (std::getline(file, line)) {
		lineContent = str_proc::split(line, ':');
		if (lineContent[0] == "size") {
			content = str_proc::split(lineContent[1], ',');
			axis->width = std::atoi(content[0].c_str());
			axis->height = std::atoi(content[1].c_str());
		}
		if (lineContent[0] == "top") {
			content = str_proc::split(lineContent[1], ',');
			axis->top = cv::Point(std::atoi(content[0].c_str()), std::atoi(content[1].c_str()));
		}
		if (lineContent[0] == "bottom") {
			content = str_proc::split(lineContent[1], ',');
			axis->bottom = cv::Point(std::atoi(content[0].c_str()), std::atoi(content[1].c_str()));
		}
	}
	return axis;
}
