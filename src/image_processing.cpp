#include "image_processing.hpp"

#include "opencv2\opencv.hpp"




cv::Mat ImageProcessorImpl::CvtColor(const cv::Mat &src, const cv::Rect &roi) {
	cv::Mat src_copy = src.clone();
	cv::Mat src_copy_roi = cv::Mat(src, roi);
	cv::Mat dst_gray_roi;
	cv::cvtColor(src_copy_roi, dst_gray_roi, CV_BGR2GRAY);
	std::vector<cv::Mat> channels(3);
	for (int i = 0; i < 3; ++i) {
		channels.push_back(dst_gray_roi);
	}
	cv::Mat dst_roi;
	cv::merge(channels, dst_roi);
	dst_roi.copyTo(src_copy_roi);
	/*
	cv::cvtColor(src_copy_roi, src_copy_roi, CV_BGR2GRAY);
	cv::cvtColor(src_copy_roi, src_copy_roi, CV_BGR2GRAY);
	*/
	return src_copy;
}

cv::Mat ImageProcessorImpl::Filter(const cv::Mat &src, const cv::Rect &roi,
								   const int kSize){
	cv::Mat src_copy = src.clone();
	cv::Mat src_copy_roi = cv::Mat(src_copy, roi);
	cv::medianBlur(src_copy_roi, src_copy_roi, kSize);
	return src_copy;
}

cv::Mat ImageProcessorImpl::DetectEdges(const cv::Mat &src, const cv::Rect &roi,
					const int filterSize, const int lowThreshold, const int ratio,
					const int kernelSize) {
	cv::Mat src_copy = src.clone();
	cv::Mat src_copy_roi = cv::Mat(src_copy, roi);
	cv::Mat src_gray_roi;
	cv::cvtColor(src_copy_roi, src_gray_roi, CV_BGR2GRAY);
	cv::Mat gray_blur;
	cv::blur(src_gray_roi, gray_blur, cv::Size(filterSize, filterSize));
	cv::Mat edges;
	cv::Canny(gray_blur, edges, lowThreshold, lowThreshold, kernelSize);
	cv::Mat dst = src.clone();
	cv::Mat dst_roi = cv::Mat(dst, roi);
	cv::Mat zeros = cv::Mat(dst_roi.size[0], dst_roi.size[1], dst_roi.type(), cv::Scalar::all(0));
	zeros.copyTo(dst_roi);
	src_copy_roi.copyTo(dst_roi, edges);
	return dst;
}

cv::Mat ImageProcessorImpl::Pixelize(const cv::Mat &src, const cv::Rect &roi,
				 const int kDivs) {
	cv::Mat src_copy = src.clone();
	cv::Mat src_copy_roi = cv::Mat(src_copy, roi);
	int block_size_x = roi.width / kDivs;
	int block_size_y = roi.height / kDivs;
	for (int rows = 0; rows < kDivs; ++rows) {
		for (int cols = 0; cols < kDivs; ++cols) {
			cv::Rect block_roi = cv::Rect(rows * block_size_x, cols * block_size_y, rows * block_size_x + block_size_x - 1, cols * block_size_y + block_size_y - 1);
			cv::Mat src_roi_block = cv::Mat(src_copy_roi, block_roi);
			cv::blur(src_roi_block, src_roi_block, block_roi.size());
		}
	}
	return src_copy;
}