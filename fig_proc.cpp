#include "fig_proc.h"


namespace fig_proc {

	void reduceTo(int size, cv::Mat &mat)
	{
		if (mat.size[0] > size && mat.size[1] > 500)
		{
			cv::Mat temp;
			float scale = mat.size[0] > mat.size[1] ? (float)size / mat.size[1] : (float)size / mat.size[0];
			resize(mat, temp, cv::Size(), scale, scale, cv::INTER_LINEAR);
			mat = temp;
		}
	}

	void img2Pts(cv::Mat image, std::vector<cv::Point> &coord) {
		unsigned char *data = (unsigned char *)(image.data);

		for (int row = 0; row < image.rows; row++) {
			for (int col = 0; col < image.cols; col++) {
				if (data[image.step * row + col] == 0) {
					coord.push_back(cv::Point(col, row));
				}
			}
		}
	}

	void findHullCircle(const std::vector<cv::Point> &imgPts, cv::Point &center, float &radius) {
		std::vector<cv::Point> hull;
		cv::convexHull((cv::Mat)imgPts, hull, false);

		/// the center point is at the average of all hull points
		center.x = 0;
		center.y = 0;
		for (std::vector<cv::Point>::const_iterator it = hull.begin(); it != hull.end(); ++it) {
			center.x += (*it).x;
			center.y += (*it).y;
		}
		center.x = center.x / hull.size();
		center.y = center.y / hull.size();

		/// then traverse all points to find out the radius
		cv::Point radiusP(center);
		radius = 0;
		float temp;
		for (std::vector<cv::Point>::const_iterator it = hull.begin(); it != hull.end(); ++it) {
			if (abs((*it).x - center.x) < abs(radiusP.x - center.x) || abs((*it).y - center.y) < abs(radiusP.y - center.y)) {
				continue;
			}
			temp = pow((*it).x - center.x, 2) + pow((*it).y - center.y, 2);
			if (temp > radius) {
				radius = temp;
				radiusP = (*it);
			}
		}
		radius = sqrt(radius);
	}

	std::vector<float> findCircularDist(const cv::Mat &image, const int binNum, const cv::Point center) {
		std::vector<float> dist(binNum);
		unsigned char *data = (unsigned char *)(image.data);
		int binIndex;
		int totP = 0;

		int high = 0, low = 0;
		int x, y;
		float len, radian;

		for (int row = 0; row < image.rows; row++) {
			for (int col = 0; col < image.cols; col++) {
				unsigned char value = data[image.step * row + col];
				x = col - center.x;
				y = center.y - row;
				if ((int)(value) != 0)
					continue;
				if (x == 0 && y == 0) {
					continue;
				}
				len = sqrt(x * x + y * y);
				radian = asin(y / len);
				if (x > 0 && y >= 0)
					radian = radian;
				else if (x <= 0 && y > 0)
					radian = _pi - radian;
				else if (x < 0 && y <= 0)
					radian = _pi - radian;
				else if (x >= 0 && y < 0)
					radian = 2 * _pi + radian;
				else
					continue;
				binIndex = floor(radian * 36 / 2 / _pi);
				if (binIndex < 0) {
					binIndex = 0;
				}
				else if (binIndex > 35) {
					binIndex = 35;
				}
				dist[binIndex] += 1;
				totP += 1;
			}
		}
		// normalize the circular distribution
		for (int i = 0; i < binNum; i++) {
			dist[i] = dist[i] / totP;
		}
		return dist;
	}

	double findLen(const cv::Point p1, const cv::Point p2) {
		return sqrt(pow(p1.x - p2.x, 2) + pow(p1.y - p2.y, 2));
	}

	double findAngle(const cv::Point p1, const cv::Point p2) {
		double angle = acos((p2.x - p1.x) / findLen(p1, p2)) * 180 / _pi;
		if (p2.y < p1.y) {
			angle = -angle;
		}
		return angle;
	}

	cv::Mat plotCircle(const cv::Mat &image, const cv::Point &center, const float &radius) {
		cv::Mat plotImg(image.clone());
		circle(plotImg, center, ceil(radius), cv::Scalar(255, 0, 0), 2);
		const std::string title = "center and radius";
		cv::namedWindow(title);
		cv::imshow(title, plotImg);
		cv::waitKey(0);
		cv::destroyWindow(title);
		return plotImg;
	}

	cv::Mat plotArrowedLine(const cv::Mat &image, const cv::Point &start, const cv::Point &end) {
		cv::Mat plotImg(image.clone());
		cv::arrowedLine(plotImg, start, end, cv::Scalar(0, 0, 255), 2);
		const std::string title = "arrowed line";
		cv::namedWindow(title);
		cv::imshow(title, plotImg);
		cv::waitKey(0);
		cv::destroyWindow(title);
		return plotImg;
	}

	cv::Mat plotLines(const cv::Mat &image, const std::vector<std::vector<cv::Point> > &lines) {
		cv::Mat plotImg(image.clone());
		for (std::vector<std::vector<cv::Point> >::const_iterator it = lines.begin(); it < lines.end(); ++it) {
			cv::line(plotImg, (*it)[0], (*it)[1], cv::Scalar(0, 0, 255), 2);
		}
		const std::string title = "lines";
		cv::namedWindow(title);
		cv::imshow(title, plotImg);
		cv::waitKey(0);
		cv::destroyWindow(title);
		return plotImg;
	}

	cv::Mat plotCircles(const cv::Mat &image, const std::vector<cv::Vec3f> &circles) {
		cv::Mat plotImg(image.clone());
		for (size_t i = 0; i < circles.size(); i++)
		{
			cv::Point center(cvRound(circles[i][0]), cvRound(circles[i][1]));
			int radius = cvRound(circles[i][2]);
			// draw the circle center
			circle(plotImg, center, 3, cv::Scalar(0, 255, 0), -1, 8, 0);
			// draw the circle outline
			circle(plotImg, center, radius, cv::Scalar(0, 0, 255), 3, 8, 0);
		}
		const std::string title = "circles";
		cv::namedWindow(title);
		cv::imshow(title, plotImg);
		cv::waitKey(0);
		cv::destroyWindow(title);
		return plotImg;
	}

	cv::Mat plotPolygon(const cv::Mat &image, const std::vector<cv::Point> &polygon) {
		cv::Mat plotImg(image.clone());
		for (std::vector<cv::Point>::size_type i = 0; i < polygon.size() - 1; i++) {
			line(plotImg, polygon[i], polygon[i + 1], cv::Scalar(0, 0, 255), 2);
		}
		line(plotImg, polygon[polygon.size() - 1], polygon[0], cv::Scalar(0, 0, 255), 2);
		const std::string title = "polygon";
		cv::imshow(title, plotImg);
		cv::waitKey(0);
		cv::destroyWindow(title);
		return plotImg;
	}

	cv::Mat plotCircularDist(const cv::Mat &image, const std::vector<float> &circularDist, const cv::Point center, const float radius) {
		cv::Mat plotImg(image.clone());
		float lineLen, angle;
		int x, y;
		cv::Point end;
		float max = 0;
		int binNum = circularDist.size();
		for (int i = 0; i < binNum; i++) {
			if (circularDist[i] > max)
				max = circularDist[i];
		}
		for (int i = 0; i < binNum; i++) {
			lineLen = radius * circularDist[i] / max;
			angle = 2 * _pi / binNum * (i + 0.5);
			x = cos(angle) * lineLen;
			y = sin(angle) * lineLen;
			end.x = center.x + (int)x;
			end.y = center.y - (int)y;
			line(plotImg, center, end, cv::Scalar(0, 0, 255), 2);
		}
		const std::string title = "circular dist";
		imshow(title, plotImg);
		cv::waitKey(0);
		cv::destroyWindow(title);
		return plotImg;
	}
}