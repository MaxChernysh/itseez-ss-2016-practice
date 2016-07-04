#include <gtest/gtest.h>

#include "opencv2/core/core.hpp"

#include <iostream>

#include "workaround.hpp"

using namespace cv;

TEST(dummy, dummy_test)
{
    Mat mat(3, 7, CV_8UC1, Scalar::all(0));
    Mat submat = mat(Rect(0, 0, 2, 3));
    EXPECT_FALSE(submat.isContinuous());
}

TEST(threshold, threshold_test) 
{
	Mat mat(3, 3, CV_8UC1, 100);
	MatrixProcessor proc;
	proc.Threshold(mat.data, mat.cols, mat.rows, 128);
	EXPECT_EQ(0.0, cv::norm(mat.reshape(1, 1), NORM_L1));
}

TEST(averaging, averaging_test) 
{
	unsigned char mat[] = { 1,1,1,1,1,1,1,1,1 };
	MatrixProcessor proc;
	unsigned char* newMat = proc.Averaging(mat, 3, 3, 1);
	EXPECT_EQ(1, (int)newMat[0]);
}