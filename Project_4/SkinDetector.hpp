#pragma once
#include <opencv2/core/mat.hpp>

class SkinDetector
{
public:
	SkinDetector(void);

	void drawSkinColorSampler(cv::UMat input);
	void calibrate(cv::UMat input);
	cv::UMat getSkinMask(cv::UMat input);
};
