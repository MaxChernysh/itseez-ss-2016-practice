#include "tracking.hpp"

#include <iostream>
#include "opencv2\imgproc.hpp"
#include "opencv2\video\tracking.hpp"

using std::string;
using std::shared_ptr;
using namespace cv;

shared_ptr<Tracker> Tracker::CreateTracker(const string &name) {
	if (name == "median_flow") {
		return std::make_shared<MedianFlowTracker>();
	}
  std::cerr << "Failed to create tracker with name '" << name << "'"
            << std::endl;
  return nullptr;
}

bool MedianFlowTracker::Init(const Mat &frame, const Rect &roi) {
	position_ = roi;
	frame_ = frame.clone();
	return true;
}


bool sortX(const cv::Point2f p1, const cv::Point2f p2) { return p1.x < p2.x; }
bool sortY(const cv::Point2f p1, const cv::Point2f p2) { return p1.y < p2.y; }

cv::Rect MedianFlowTracker::Track(const Mat &frame) {
	std::vector<Point2f> oldPoints, newPoints, goodPoints;
	std::vector<uchar> status;
	std::vector<float> err;
	Mat frameRoi = Mat(frame_, position_);
	goodFeaturesToTrack(frameRoi, oldPoints, 100, 0.01, 5);
	for (int i = 0; i < oldPoints.size(); ++i) {
		oldPoints[i].x += position_.x;
		oldPoints[i].y += position_.y;
	}
	calcOpticalFlowPyrLK(frame_, frame, oldPoints, newPoints, status, err);
	int stat = 0;
	for (int i = 0; i < newPoints.size(); ++i) {
		if (status[i]) {
			goodPoints.push_back(Point2f(newPoints[i].x - oldPoints[i].x, newPoints[i].y - oldPoints[i].y));
			++stat;
		}
	}
	std::sort(goodPoints.begin(), goodPoints.end(), sortX);
	position_.x += goodPoints[stat / 2 - 1].x;
	std::sort(goodPoints.begin(), goodPoints.end(), sortY);
	position_.y += goodPoints[stat / 2 - 1].x;
	frame_ = frame.clone();
	return position_;
}
