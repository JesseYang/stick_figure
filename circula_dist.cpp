#include "circular_dist.h"

CircularDist::CircularDist(MyObject *obj) : Feature(obj) {
	this->binNum = 36;
}

CircularDist::CircularDist() : Feature() {
	this->binNum = 36;
}

void CircularDist::calculate() {
	fig_proc::findHullCircle(this->obj->imgPts, this->center, this->radius);
	this->distAry = fig_proc::findCircularDist(this->obj->transImg, this->binNum, this->center);
}

void CircularDist::plot() {
	cv::Mat colorTransImg;
	cv::cvtColor(this->obj->transImg, colorTransImg, cv::COLOR_GRAY2BGR);
	fig_proc::plotCircularDist(colorTransImg, this->distAry, this->center, this->radius);
}

void CircularDist::updateStd() {
	std::ofstream file(file_proc::getPathByName(this->obj->objName(), file_proc::STD_FEATURE));

	file << "size:" << this->width << "," << this->height << std::endl;
	file << "center:" << this->center.x << "," << this->center.y << std::endl;
	file << "radius:" << this->radius << std::endl;
	file << "distAry:";
	for (int i = 0; i < this->binNum; i++) {
		file << this->distAry[i];
		if (i != this->binNum - 1) {
			file << ",";
		}
	}
	file << std::endl;
	file.close();
}

double CircularDist::mainDir() {
	int mainDirIndex = -1;
	float max = -1.0f;
	for (std::vector<float>::iterator it = distAry.begin(); it != distAry.end(); ++it) {
		if ((*it) > max) {
			max = (*it);
			mainDirIndex = it - distAry.begin();
		}
	}
	double angle = (mainDirIndex + 0.5) * 360.0 / this->binNum;
	return angle;
}

cv::Mat CircularDist::calRotMat(Feature *feature)
{
	CircularDist *circularDist = (CircularDist *)feature;
	double scale = circularDist->radius / this->radius;
	double angle = circularDist->mainDir() - this->mainDir();
	this->transMat = cv::getRotationMatrix2D(this->center, angle, scale);
	return this->transMat;
}

cv::Rect CircularDist::getRoi(Feature *feature)
{
	int xStart = this->center.x - feature->width / 2;
	int yStart = this->center.y - feature->height / 2;
	cv::Rect roi(xStart, yStart, feature->width, feature->height);
	return roi;
}

CircularDist *CircularDist::loadStdFeature(std::string objName) {
	std::ifstream file(file_proc::getPathByName(objName, file_proc::STD_FEATURE));
	std::string line;
	std::vector<std::string> lineContent;
	std::vector<std::string> content;
	// Todo: read data from file
	CircularDist *dist = new CircularDist();
	while (std::getline(file, line)) {
		lineContent = str_proc::split(line, ':');
		if (lineContent[0] == "size") {
			content = str_proc::split(lineContent[1], ',');
			dist->width = std::atoi(content[0].c_str());
			dist->height = std::atoi(content[1].c_str());
		}
		if (lineContent[0] == "center") {
			content = str_proc::split(lineContent[1], ',');
			dist->center = cv::Point(std::atoi(content[0].c_str()), std::atoi(content[1].c_str()));
		}
		if (lineContent[0] == "radius") {
			dist->radius = std::atof(lineContent[1].c_str());
		}
		if (lineContent[0] == "distAry") {
			content = str_proc::split(lineContent[1], ',');
			for (std::vector<std::string>::iterator it = content.begin(); it != content.end(); ++it) {
				dist->distAry.push_back(std::atof((*it).c_str()));
			}
		}
	}
	return dist;
}
