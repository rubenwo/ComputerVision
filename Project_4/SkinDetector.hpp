#pragma once
#include <opencv2/core/mat.hpp>

class SkinDetector
{
public:
	SkinDetector(void);

	void drawSkinColorSampler(cv::Mat input);
	void calibrate(cv::Mat input);
	cv::Mat getSkinMask(cv::Mat input);
};
