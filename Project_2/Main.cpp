#include <opencv2/core.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/opencv.hpp>
#include <iostream>
#include <opencv2/imgproc/imgproc.hpp>


int main(int argc, char* argv[])
{
	cv::SimpleBlobDetector::Params params;


	//params.filterByArea = true;
	//params.minArea = 1500;

	//params.filterByCircularity = true;
	//params.minCircularity = 0.8;
	//params.maxCircularity = 1;


	//params.filterByColor = true;
	//params.blobColor = 0;
	params.filterByConvexity = true;

	params.minConvexity = 0.8;
	params.maxConvexity = 1;

	cv::VideoCapture cap;
	cv::Ptr<cv::SimpleBlobDetector> detector = cv::SimpleBlobDetector::create(params);

	int deviceID = 0;
	int apiID = cv::CAP_ANY;


	cap.open(deviceID + apiID);

	if (!cap.isOpened())
	{
		std::cerr << "ERROR! Unable to open camera\n";
		return -1;
	}


	cv::Mat frame;
	cv::Mat threshold_frame;
	cv::Mat eroded_frame;
	cv::Mat frame_with_key_points;
	std::cout << "Start grabbing" << std::endl << "Press any key to terminate" << std::endl;
	for (;;)
	{
		cap.read(frame);

		if (frame.empty())
		{
			std::cerr << "ERROR! Blank frame grabbed\n";
			break;
		}

		cv::Mat element = cv::getStructuringElement(cv::MORPH_CROSS,
		                                            cv::Size(3, 3),
		                                            cv::Point(0, 0));


		cv::threshold(frame, threshold_frame, 75, 150, cv::THRESH_BINARY);

		cv::erode(threshold_frame, eroded_frame, element, cv::Point(-1, -1), 3);
		std::vector<cv::KeyPoint> key_points;
		detector->detect(eroded_frame, key_points);
		drawKeypoints(frame, key_points, frame_with_key_points, cv::Scalar(0, 0, 255),
		              cv::DrawMatchesFlags::DRAW_RICH_KEYPOINTS);


		//cv::imshow("Live", frame);
		cv::imshow("KeyPoints", frame_with_key_points);
		cv::waitKey(16);
	}
	return 0;
}
