#pragma once
#include <opencv2/core/mat.hpp>

class BackgroundRemover
{
public:
	BackgroundRemover(void);
	void calibrate(cv::UMat input);
	cv::UMat getForeground(cv::UMat input);

private:
	cv::UMat background;
	bool calibrated = false;

	cv::UMat getForegroundMask(cv::UMat input);
	void removeBackground(cv::UMat input, cv::UMat background);
};
