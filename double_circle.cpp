#include "double_circle.h"

DoubleCircle::DoubleCircle(MyObject *obj) : Feature(obj) {
}

DoubleCircle::DoubleCircle() : Feature() {
}

void DoubleCircle::calculate() {
	std::vector<cv::Vec3f> circles;
	int minR = 0;
	int maxR = 0;
	int cannyThreshold = 10;
	int minDist = 30;
	cv::HoughCircles(this->obj->blurImg, circles, CV_HOUGH_GRADIENT, 1, minDist, cannyThreshold, 45, minR, maxR);

	this->center1.x = cvRound(circles[0][0]);
	this->center1.y = cvRound(circles[0][1]);
	this->radius1 = circles[0][2];
	this->center2.x = cvRound(circles[1][0]);
	this->center2.y = cvRound(circles[1][1]);
	this->radius2 = circles[1][2];

	float angle = asin((center2.y - center1.y) / this->centerDist()) * 180 / _pi;

	cv::Mat trans = cv::getRotationMatrix2D(this->center1, angle, 1.0f);
	std::vector<cv::Point> points;
	std::vector<cv::Point> centers;
	centers.push_back(this->center1);
	centers.push_back(this->center2);
	cv::transform(centers, centers, trans);
	cv::transform(this->obj->imgPts, points, trans);
	int y = centers[0].y;

	int above = 0, below = 0;
	for (std::vector<cv::Point>::iterator it = points.begin(); it < points.end(); ++it) {
		if ((*it).y < y)
			above++;
		if ((*it).y > y)
			below++;
	}
	if ((above - below) * (centers[1].x - centers[0].x) > 0) {
		cv::Point tmpP;
		tmpP = this->center1;
		this->center1 = this->center2;
		this->center2 = tmpP;
		float tmpR;
		tmpR = this->radius1;
		this->radius1 = this->radius2;
		this->radius2 = tmpR;
	}
}

void DoubleCircle::plot() {
	std::vector<cv::Vec3f> circles;
	circles.push_back(cv::Vec3f(this->center1.x, this->center1.y, this->radius1));
	circles.push_back(cv::Vec3f(this->center2.x, this->center2.y, this->radius2));
	cv::Mat colorTransImg;
	cv::cvtColor(this->obj->transImg, colorTransImg, cv::COLOR_GRAY2BGR);
	fig_proc::plotCircles(colorTransImg, circles);
}

void DoubleCircle::updateStd() {
	std::ofstream file(file_proc::getPathByName(this->obj->objName(), file_proc::STD_FEATURE));

	file << "size:" << this->width << "," << this->height << std::endl;
	file << "circle1:" << this->center1.x << "," << this->center1.y << "," << this->radius1 << std::endl;
	file << "circle2:" << this->center2.x << "," << this->center2.y << "," << this->radius2 << std::endl;

	file.close();
}

cv::Mat DoubleCircle::calRotMat(Feature *feature)
{
	DoubleCircle *dblCircle = (DoubleCircle *)feature;
	double scale = dblCircle->centerDist() / this->centerDist();
	cv::Point center((this->center1.x + this->center2.x) / 2, (this->center1.y + this->center2.y) / 2);
	double angle1 = fig_proc::findAngle(dblCircle->center2, dblCircle->center1);
	double angle2 = fig_proc::findAngle(this->center2, this->center1);
	double angle = angle2 - angle1;
	this->transMat = cv::getRotationMatrix2D(center, angle, scale);
	return this->transMat;
}

cv::Rect DoubleCircle::getRoi(Feature *feature)
{
	DoubleCircle *dblCircle = (DoubleCircle *)feature;
	std::vector<cv::Point> v1;
	v1.push_back(cv::Point((this->center1.x + this->center2.x) / 2, (this->center1.y + this->center2.y) / 2));
	std::vector<cv::Point> v2;
	cv::Point transCenter;
	cv::transform(v1, v2, this->transMat);
	int xStart = v2[0].x - (dblCircle->center1.x + dblCircle->center2.x) / 2;
	int yStart = v2[0].y - (dblCircle->center1.y + dblCircle->center2.y) / 2;
	cv::Rect roi(xStart, yStart, feature->width, feature->height);
	return roi;
}

double DoubleCircle::centerDist()
{
	return sqrt(pow(this->center1.x - this->center2.x, 2) + pow(this->center1.y - this->center2.y, 2));
}

DoubleCircle *DoubleCircle::loadStdFeature(std::string objName) {
	std::ifstream file(file_proc::getPathByName(objName, file_proc::STD_FEATURE));
	std::string line;
	std::vector<std::string> lineContent;
	std::vector<std::string> content;
	// Todo: read data from file
	DoubleCircle *dblCircle = new DoubleCircle();
	while (std::getline(file, line)) {
		lineContent = str_proc::split(line, ':');
		if (lineContent[0] == "size") {
			content = str_proc::split(lineContent[1], ',');
			dblCircle->width = std::atoi(content[0].c_str());
			dblCircle->height = std::atoi(content[1].c_str());
		}
		if (lineContent[0] == "circle1") {
			content = str_proc::split(lineContent[1], ',');
			dblCircle->center1 = cv::Point(std::atoi(content[0].c_str()), std::atoi(content[1].c_str()));
			dblCircle->radius1 = std::atof(content[2].c_str());
		}
		if (lineContent[0] == "circle2") {
			content = str_proc::split(lineContent[1], ',');
			dblCircle->center2 = cv::Point(std::atoi(content[0].c_str()), std::atoi(content[1].c_str()));
			dblCircle->radius2 = std::atof(content[2].c_str());
		}
	}
	return dblCircle;
}
