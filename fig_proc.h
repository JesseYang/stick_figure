#pragma once

#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

#define _pi 3.1415927

namespace fig_proc {
	void reduceTo(int, cv::Mat &);
	void img2Pts(cv::Mat, std::vector<cv::Point> &);
	void findHullCircle(const std::vector<cv::Point> &imgPts, cv::Point &, float &);
	std::vector<float> findCircularDist(const cv::Mat &, const int, const cv::Point);
	double findAngle(const cv::Point p1, const cv::Point p2);
	double findLen(const cv::Point p1, const cv::Point p2);

	cv::Mat plotCircle(const cv::Mat &, const cv::Point &, const float &);
	cv::Mat plotArrowedLine(const cv::Mat &, const cv::Point &, const cv::Point &);
	cv::Mat plotLines(const cv::Mat &, const std::vector<std::vector<cv::Point> > &);
	cv::Mat plotCircles(const cv::Mat &, const std::vector<cv::Vec3f> &);
	cv::Mat plotPolygon(const cv::Mat &, const std::vector<cv::Point> &);
	cv::Mat plotCircularDist(const cv::Mat &, const std::vector<float> &, const cv::Point, const float);
}