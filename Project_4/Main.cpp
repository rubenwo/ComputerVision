#include <iostream>
#include <opencv2/videoio.hpp>
#include <opencv2/video/tracking.hpp>
#include <opencv2/highgui.hpp>
#include "FingerCounter.hpp"
#include "FaceDetector.hpp"
#include "SkinDetector.hpp"
#include "BackgroundRemover.hpp"
#include "../../../../../../../../../../opencv/build/include/opencv2/flann/dist.h"
#include "../../../../../../../../../../opencv/build/include/opencv2/imgproc.hpp"
#include "../../../../../../../../../../opencv/build/include/opencv2/core.hpp"
#define _USE_MATH_DEFINES
#include <math.h>

cv::UMat frame, blurred, hsv, skin_mask;

int main(int argc, char* argv[])
{
	cv::VideoCapture vcap("../resources/hand.mp4");
	// cv::VideoCapture vcap(0);
	auto skin_lower = cv::Scalar(0, 0.10 * 255, 50);
	auto skin_upper = cv::Scalar(50, 0.68 * 255, 255);

	cv::useOpenVX();
	FingerCounter fingerCounter;

	cv::Mat element = getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(4, 4));

	for (;;)
	{
		if (vcap.read(frame))
		{
			//-----Pre-Process frame-----//
			cv::resize(frame, frame, cv::Size(720, 1240));
			//cv::flip(frame, frame, +1);

			//cv::GaussianBlur(frame, blurred, cv::Size(11, 11), 0); //Blur the image in order to remove noise.
			cv::medianBlur(frame, blurred, 7);

			cv::cvtColor(blurred, hsv, CV_BGR2HSV); //Convert the blurred image to HSV.

			cv::inRange(hsv, skin_lower, skin_upper, skin_mask); //Create a binary output with only the color of the person's.

			cv::dilate(skin_mask, skin_mask, element, cv::Point(-1, -1), 4);
			cv::erode(skin_mask, skin_mask, element, cv::Point(-1, -1), 2);

			fingerCounter.findFingersCount(skin_mask, frame);

			//-----Show results-----//
			cv::imshow("Original", frame);
			cv::imshow("Blurred", blurred);
			cv::imshow("HSV", hsv);
			cv::imshow("Skin Mask", skin_mask);
		}
		else
		{
			std::cout << "No more frames to read" << std::endl;
			break;
		}
		int key = cv::waitKey(1);
	}

	return 0;
}
