#pragma once
#include <opencv2/core/mat.hpp>

class FingerCounter
{
public:
	FingerCounter(void);
	cv::Mat findFingersCount(cv::Mat input_image, cv::Mat frame);
};
