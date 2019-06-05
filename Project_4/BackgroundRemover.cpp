#include "BackgroundRemover.hpp"

BackgroundRemover::BackgroundRemover()
{
	background;
	calibrated = false;
}

void BackgroundRemover::calibrate(cv::UMat input)
{
}

cv::UMat BackgroundRemover::getForeground(cv::UMat input)
{
	return input;
}

cv::UMat BackgroundRemover::getForegroundMask(cv::UMat input)
{
	return input;

}

void BackgroundRemover::removeBackground(cv::UMat input, cv::UMat background)
{
}
