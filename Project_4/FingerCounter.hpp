#pragma once
#include <opencv2/core/mat.hpp>

class FingerCounter
{
public:
	FingerCounter(void);
	cv::UMat findFingersCount(cv::UMat input_image, cv::UMat frame);
};
