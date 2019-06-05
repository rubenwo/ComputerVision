#pragma once

#include <opencv2/core/mat.hpp>

class FaceDetector
{
public:
	FaceDetector(void);
	void removeFaces(cv::UMat input, cv::UMat output);
};
