#include "horizon_line.h"

HorizonLine::HorizonLine(MyObject *obj) : Feature(obj) {
}

HorizonLine::HorizonLine() : Feature() {
}

void HorizonLine::calculate() {
	std::vector<cv::Vec4i> lines;
	cv::Mat tmp;

	cv::bitwise_not(this->obj->erodeImg, tmp);
	int threshold = 50;
	cv::HoughLinesP(tmp, lines, 1, _pi / 180, threshold, 30, 20);
	std::vector<std::vector<cv::Point> > linePts;
	for (std::vector<cv::Vec4i>::iterator it = lines.begin(); it < lines.end(); ++it) {
		std::vector<cv::Point> oneLine;
		oneLine.push_back(cv::Point((*it)[0], (*it)[1]));
		oneLine.push_back(cv::Point((*it)[2], (*it)[3]));
		linePts.push_back(oneLine);
	}

	/// find out the longest line
	cv::Point p1, p2;
	double len = 0, cur_len;
	for (std::vector<std::vector<cv::Point> >::iterator it = linePts.begin(); it < linePts.end(); ++it) {
		cur_len = fig_proc::findLen((*it)[0], (*it)[1]);
		if (cur_len > len) {
			len = cur_len;
			p1 = (*it)[0];
			p2 = (*it)[1];
		}
	}

	/// judge which point is on the left and which is on the right
	float angle = asin((p2.y - p1.y) / len) * 180 / _pi;

	cv::Mat trans = cv::getRotationMatrix2D(p1, angle, 1.0f);
	std::vector<cv::Point> endPts;
	std::vector<cv::Point> points;
	endPts.push_back(p1);
	endPts.push_back(p2);
	cv::transform(endPts, endPts, trans);
	cv::transform(this->obj->imgPts, points, trans);
	int y = endPts[0].y;

	int above = 0, below = 0;
	for (std::vector<cv::Point>::iterator it = points.begin(); it < points.end(); ++it) {
		if ((*it).y < y)
			above++;
		if ((*it).y > y)
			below++;
	}
	if ((above - below) * (endPts[1].x - endPts[0].x) > 0) {
		this->left = p1;
		this->right = p2;
	}
	else {
		this->left = p1;
		this->right = p2;
	}
}

void HorizonLine::plot() {
	cv::Mat colorTransImg;
	cv::cvtColor(this->obj->transImg, colorTransImg, cv::COLOR_GRAY2BGR);
	fig_proc::plotArrowedLine(colorTransImg, this->left, this->right);
}

void HorizonLine::updateStd() {
	std::ofstream file(file_proc::getPathByName(this->obj->objName(), file_proc::STD_FEATURE));

	file << "size:" << this->width << "," << this->height << std::endl;
	file << "left:" << this->left.x << "," << this->left.y << std::endl;
	file << "right:" << this->right.x << "," << this->right.y << std::endl;
	file.close();
}

cv::Mat HorizonLine::calRotMat(Feature *feature)
{
	HorizonLine *horizonLine = (HorizonLine *)feature;
	double scale = horizonLine->lineLen() / this->lineLen();
	cv::Point center(this->right.x + this->left.x / 2, this->right.y + this->left.y / 2);
	double angle1 = fig_proc::findAngle(horizonLine->left, horizonLine->right);
	double angle2 = fig_proc::findAngle(this->left, this->right);
	double angle = angle2 - angle1;
	this->transMat = cv::getRotationMatrix2D(center, angle, scale);
	return this->transMat;
}

cv::Rect HorizonLine::getRoi(Feature *feature)
{
	HorizonLine *horizonLine = (HorizonLine *)feature;
	std::vector<cv::Point> v1;
	v1.push_back(cv::Point((this->right.x + this->left.x) / 2, (this->right.y + this->left.y) / 2));
	std::vector<cv::Point> v2;
	cv::Point transCenter;
	cv::transform(v1, v2, this->transMat);
	int xStart = v2[0].x - (horizonLine->right.x + horizonLine->left.x) / 2;
	int yStart = v2[0].y - (horizonLine->right.y + horizonLine->left.y) / 2;
	cv::Rect roi(xStart, yStart, feature->width, feature->height);
	return roi;
}

double HorizonLine::lineLen()
{
	return sqrt(pow(this->right.x - this->left.x, 2) + pow(this->right.y - this->left.y, 2));
}

HorizonLine *HorizonLine::loadStdFeature(std::string objName) {
	std::ifstream file(file_proc::getPathByName(objName, file_proc::STD_FEATURE));
	std::string line;
	std::vector<std::string> lineContent;
	std::vector<std::string> content;
	// Todo: read data from file
	HorizonLine *horizonLine = new HorizonLine();
	while (std::getline(file, line)) {
		lineContent = str_proc::split(line, ':');
		if (lineContent[0] == "size") {
			content = str_proc::split(lineContent[1], ',');
			horizonLine->width = std::atoi(content[0].c_str());
			horizonLine->height = std::atoi(content[1].c_str());
		}
		if (lineContent[0] == "right") {
			content = str_proc::split(lineContent[1], ',');
			horizonLine->right = cv::Point(std::atoi(content[0].c_str()), std::atoi(content[1].c_str()));
		}
		if (lineContent[0] == "left") {
			content = str_proc::split(lineContent[1], ',');
			horizonLine->left = cv::Point(std::atoi(content[0].c_str()), std::atoi(content[1].c_str()));
		}
	}
	return horizonLine;
}
