#include <iostream>
#include <opencv2/videoio.hpp>
#include <opencv2/video/tracking.hpp>
#include <opencv2/highgui.hpp>
#include "FingerCounter.hpp"
#include "FaceDetector.hpp"
#include "SkinDetector.hpp"
#include "BackgroundRemover.hpp"

cv::UMat frame, blurred, hsv, skin_mask;


void drawContours(std::vector<cv::Mat>& contours, std::vector<cv::Vec4i>& hierarchy)
{
	std::vector<cv::Moments> mu(contours.size());

	for (int i = 0; i < contours.size(); i++)
	{
		mu[i] = cv::moments(contours[i], false); //Add the contours to the moments vector
	}

	// get the centroid of the objects.
	std::vector<cv::Point2f> mc(contours.size());
	for (int i = 0; i < contours.size(); i++)
	{
		mc[i] = cv::Point2f(mu[i].m10 / mu[i].m00, mu[i].m01 / mu[i].m00);
	}

	// draw the contours of the objects.
	for (int i = 0; i < contours.size(); i++)
	{
		cv::Scalar color = cv::Scalar(0, 0, 255); // B G R values
		drawContours(frame, contours, i, color, 2, 8, hierarchy, 0, cv::Point());
		circle(frame, mc[i], 4, color, -1, 8, 0);
	}
}

int main(int argc, char* argv[])
{
	cv::VideoCapture vcap("../resources/hand.mp4");
	auto skin_lower = cv::Scalar(0, 0.23 * 255, 50);
	auto skin_upper = cv::Scalar(50, 0.68 * 255, 255);

	std::vector<cv::Mat> contours;
	std::vector<cv::Vec4i> hierarchy;

	cv::useOpenVX();

	BackgroundRemover backgroundRemover;
	SkinDetector skinDetector;
	FaceDetector faceDetector;
	FingerCounter fingerCounter;

	cv::Mat element = getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(4, 4));

	for (;;)
	{
		contours.clear();
		hierarchy.clear();
		if (vcap.read(frame))
		{
			//-----Pre-Process frame-----//
			cv::resize(frame, frame, cv::Size(720, 1240));
			//cv::flip(frame, frame, +1);

			//cv::GaussianBlur(frame, blurred, cv::Size(11, 11), 0); //Blur the image in order to remove noise.
			cv::medianBlur(frame, blurred, 7);

			cv::cvtColor(blurred, hsv, CV_BGR2HSV); //Convert the blurred image to HSV.

			cv::inRange(hsv, skin_lower, skin_upper, skin_mask); //Create a binary output with only the blue's.

			cv::dilate(skin_mask, skin_mask, element, cv::Point(-1, -1), 4);
			cv::erode(skin_mask, skin_mask, element, cv::Point(-1, -1), 2);


			cv::findContours(skin_mask, contours, hierarchy, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE);

			drawContours(contours, hierarchy);
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
