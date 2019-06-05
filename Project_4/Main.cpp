#include <iostream>
#include <opencv2/videoio.hpp>
#include <opencv2/video/tracking.hpp>
#include <opencv2/highgui.hpp>
#include "FingerCounter.hpp"
#include "FaceDetector.hpp"
#include "SkinDetector.hpp"
#include "BackgroundRemover.hpp"

cv::UMat frame, scaled, flipped, fingerCount;


int main(int argc, char* argv[])
{
	cv::VideoCapture vcap(0);

	cv::useOpenVX();

	BackgroundRemover backgroundRemover;
	SkinDetector skinDetector;
	FaceDetector faceDetector;
	FingerCounter fingerCounter;
	cv::UMat frame, frameOut, handMask, foreground, fingerCountDebug;


	for (;;)
	{
		vcap >> frame;

		//-----Pre-Process frame-----//
		cv::resize(frame, frame, cv::Size(1240, 720));
		cv::flip(frame, frame, +1);

		frameOut = frame.clone();
		//-----Detect Skin-----//
		skinDetector.drawSkinColorSampler(frameOut);

		foreground = backgroundRemover.getForeground(frame);

		//-----Remove Face-----//
		faceDetector.removeFaces(frame, foreground);

		//-----Create Finger Mask-----//
		handMask = skinDetector.getSkinMask(foreground);
		fingerCountDebug = fingerCounter.findFingersCount(handMask, frameOut);


		//-----Show results-----//
		cv::imshow("output", frameOut);
		cv::imshow("foreground", foreground);
		cv::imshow("handMask", handMask);
		cv::imshow("handDetection", fingerCountDebug);

		int key = cv::waitKey(1);

		if (key == 27) // esc
			break;
		else if (key == 98) // b
			backgroundRemover.calibrate(frame);
		else if (key == 115) // s
			skinDetector.calibrate(frame);
	}

	return 0;
}
