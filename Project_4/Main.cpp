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

int GetAngleABC(cv::Point a, cv::Point b, cv::Point c)
{
	cv::Point ab = { b.x - a.x, b.y - a.y };
	cv::Point cb = { b.x - c.x, b.y - c.y };

	float dot = (ab.x * cb.x + ab.y * cb.y); // dot product
	float cross = (ab.x * cb.y - ab.y * cb.x); // cross product

	float alpha = atan2(cross, dot);

	return (int)floor(alpha * 180. / M_PI + 0.5);
}

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

int main(int argc, char* argv[]) try
{
	cv::VideoCapture vcap("../resources/hand.mp4");
	auto skin_lower = cv::Scalar(0, 0.23 * 255, 50);
	auto skin_upper = cv::Scalar(50, 0.68 * 255, 255);

	std::vector<std::vector<cv::Point>> contours;
	std::vector<cv::Point> contourPoints;

	std::vector<cv::Vec4i> hierarchy;

	cv::useOpenVX();

	BackgroundRemover backgroundRemover;
	SkinDetector skinDetector;
	FaceDetector faceDetector;
	FingerCounter fingerCounter;

	cv::Mat element = getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(4, 4));

	for (;;)
	{
		int fingers = 0;
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



			int lrgctridx = 0;
			int maxarea = 0;
			for (int i = 0; i < contours.size(); i++)
			{
				double a = cv::contourArea(contours[i]);
				if (a > maxarea)
				{
					maxarea = a;
					lrgctridx = i;
				}
			}

			std::vector<std::vector<cv::Point>> conHull(contours.size());
			std::vector<std::vector<int> >inthull(contours.size());
			std::vector<std::vector<cv::Vec4i>> defects(contours.size());
			for (int i = 0; i < contours.size(); i++) { 
				convexHull(contours[i], conHull[i]);
				convexHull(contours[i], inthull[i]);
				if (conHull[i].size() > 3)
				{
					convexityDefects(contours[i], inthull[i], defects[i]);
				}
			}

			if (maxarea > 100) {
				/// Draw convexityDefects
				for (int j = 0; j < defects[lrgctridx].size(); ++j)
				{
					const cv::Vec4i& v = defects[lrgctridx][j];
					float depth = v[3] / 256;

					if (depth > 10) //  filter defects by depth
					{
						int startidx = v[0];
						cv::Point2i ptStart(contours[lrgctridx][startidx]);
						int endidx = v[1];
						cv::Point ptEnd(contours[lrgctridx][endidx]);
						int faridx = v[2];
						cv::Point ptFar(contours[lrgctridx][faridx]);

						int angle = GetAngleABC(ptStart, ptEnd, ptFar);

						if (angle <= 90 && angle > 0) //only draw fingers that are not folded
						{
							fingers++;
							line(frame, ptStart, ptFar, cv::Scalar(255, 0, 0), 1);
							line(frame, ptEnd, ptFar, cv::Scalar(0, 0, 255), 1);
							circle(frame, ptFar, 4, cv::Scalar(0, 255, 0), 2);
						}
					}
				}
			}

			// drawContours(contours, hierarchy); //Not needed to test

			//-----Show results-----//
			cv::imshow("Original", frame);
			// cv::imshow("Blurred", blurred);
			// cv::imshow("HSV", hsv);
			// cv::imshow("Skin Mask", skin_mask);
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
catch (cv::Exception e)
{
	std::cout << "An exception occurred. Exception Nr. " << e.err << '\n';
}

