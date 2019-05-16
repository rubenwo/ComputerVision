#include <iostream>
#include <opencv2/highgui.hpp>
#include <opencv2/core/ocl.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/video/tracking.hpp>


cv::Scalar blue_lower, blue_upper;
cv::UMat frame, blurred, hsv;
cv::UMat blue_mask;

int main(int argc, char* argv[])
{
	blue_lower = cv::Scalar(100, 55, 55);
	blue_upper = cv::Scalar(130, 255, 255);

	cv::useOpenVX();

	// Open video resource
	cv::VideoCapture vcap("../resources/vid.mp4");

	if (!vcap.isOpened())
	{
		std::cout << "Cannot open the video" << std::endl;
		return -1;
	}
	// Create structuring element once
	cv::Mat element = getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(4, 4));

	// processing loop
	for (;;)
	{
		std::vector<cv::Mat> blue_contours;
		std::vector<cv::Vec4i> hierarchy;

		if (vcap.read(frame))
		{
			cv::resize(frame, frame, cv::Size(540, 960)); //Resize the image to a easier resolution.

			cv::GaussianBlur(frame, blurred, cv::Size(11, 11), 0); //Blur the image in order to remove noise.

			cv::cvtColor(blurred, hsv, CV_BGR2HSV); //Convert the blurred image to HSV.

			cv::inRange(hsv, blue_lower, blue_upper, blue_mask); //Create a binary output with only the blue's.


			//Erode & Dilate to remove smaller objects
			cv::erode(blue_mask, blue_mask, element, cv::Point(-1, -1), 5);
			cv::dilate(blue_mask, blue_mask, element, cv::Point(-1, -1), 5);

			cv::findContours(blue_mask, blue_contours, hierarchy, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE);
			//Find the contours of the blue objects.
			std::vector<cv::Moments> mu(blue_contours.size());

			for (int i = 0; i < blue_contours.size(); i++)
			{
				mu[i] = cv::moments(blue_contours[i], false); //Add the contours to the moments vector
			}

			// get the centroid of the objects.
			std::vector<cv::Point2f> mc(blue_contours.size());
			for (int i = 0; i < blue_contours.size(); i++)
			{
				mc[i] = cv::Point2f(mu[i].m10 / mu[i].m00, mu[i].m01 / mu[i].m00);
			}

			// draw the contours of the objects.
			for (int i = 0; i < blue_contours.size(); i++)
			{
				cv::Scalar color = cv::Scalar(0, 0, 255); // B G R values
				drawContours(frame, blue_contours, i, color, 2, 8, hierarchy, 0, cv::Point());
				circle(frame, mc[i], 4, color, -1, 8, 0);
			}


			cv::imshow("Contours", frame); //Finally show the image with the contours
		}
		else
		{
			std::cout << "couldn't read frame" << std::endl;
			break;
		}
		if (cv::waitKey(1) == 27)
		{
			break;
		}
	}

	return 0;
}
