#include "my_object.h"

MyObject::MyObject(cv::Mat oriImg) {
	fig_proc::reduceTo(500, oriImg);
	this->oriImg = oriImg;
	this->img = oriImg >= 255 * 3 / 5;
	cv::cvtColor(this->img, this->img, cv::COLOR_BGR2GRAY);
	int expand = (this->img.rows > this->img.cols ? this->img.rows : this->img.cols) / 2;
	int expand_rows = 2 * expand + this->img.rows;
	int expand_cols = 2 * expand + this->img.cols;
	this->transImg = cv::Mat(expand_rows, expand_cols, CV_8UC1, cv::Scalar::all(255));
	this->img.copyTo(this->transImg(cv::Rect(expand, expand, this->img.cols, this->img.rows)));
	cv::GaussianBlur(this->transImg, this->blurImg, cv::Size(9, 9), 2, 2);

	int erosion_type = cv::MORPH_CROSS;
	int erosion_size = 2;
	cv::Mat element = cv::getStructuringElement(erosion_type,
		cv::Size(2 * erosion_size + 1, 2 * erosion_size + 1),
		cv::Point(erosion_size, erosion_size));
	erode(this->transImg, this->erodeImg, element);

	fig_proc::img2Pts(this->transImg, this->imgPts);
}

void MyObject::calFeature() {
	this->feature->calculate();
}

void MyObject::plotFeature() {
	this->feature->plot();
}

void MyObject::transform() {
	/// first get rotation transform matrix
	this->feature->calRotMat(this->stdFeature);
	cv::warpAffine(this->transImg, this->transImg, this->feature->transMat, this->transImg.size());
	this->transImg = this->transImg(this->feature->getRoi(this->stdFeature));
}

void MyObject::updateStdFeature() {
	this->feature->updateStd();
}

void MyObject::evaluate() {
	// plot the standard figure and the test figure in one image
	cv::Mat stdImage = cv::imread(file_proc::getPathByName(this->objName(), file_proc::STD_FIG));
	cvtColor(stdImage, stdImage, cv::COLOR_BGR2GRAY);
	cv::Mat blendImage;
	/*
	addWeighted(stdImage, 0.5, this->transImg, 0.5, 0, blendImage);
	const std::string title = "blend image";
	cv::namedWindow(title);
	cv::imshow(title, blendImage);
	cv::waitKey(0);
	cv::destroyWindow(title);
	*/


	cv::Mat evalMat;
	cv::blur(this->transImg, evalMat, cv::Size(10, 10), cv::Point(-1, -1));
	evalMat = evalMat > 250;

	cv::Mat effectImg(stdImage.size(), CV_8UC3, cv::Scalar(0, 0, 255));
	effectImg.setTo(cv::Scalar(255, 255, 255), stdImage > 200);

	cv::Mat mask;
	cv::bitwise_or(stdImage, evalMat, mask);
	cv::bitwise_not(mask, mask);
	effectImg.setTo(cv::Scalar(0, 255, 0), mask > 50);

	cv::Mat tmp;
	cv::cvtColor(this->transImg, tmp, cv::COLOR_GRAY2BGR);
	addWeighted(effectImg, 0.7, tmp, 0.3, 0, blendImage);
	const std::string title = "blend image";
	cv::namedWindow(title);
	cv::imshow(title, blendImage);
	cv::waitKey(0);
	cv::destroyWindow(title);
}
