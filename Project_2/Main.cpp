#include <iostream>
#include "BlobDetection.h"
#include <opencv2/opencv.hpp>
#include "opencv2/imgproc/imgproc.hpp" 
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/highgui.hpp"
#include <iostream>

using namespace cv;
using namespace std;

void detectBlobs(string);

Mat labeledImage, binary16S, frame, greyscale, binaryImage, erosion_dst, dilation_dst;

int main(int argc, char* argv[])
{
	SimpleBlobDetector::Params params;
	// Change thresholds
	params.minThreshold = 200;
	params.maxThreshold = 255;

	// Filter by Area.
	params.filterByArea = true;
	params.minArea = 300;

	// Filter by Circularity
	params.filterByCircularity = true;
	params.minCircularity = 0.1;

	// Filter by Convexity
	params.filterByConvexity = true;
	params.minConvexity = 0.87;

	// Filter by Inertia
	params.filterByInertia = true;
	params.minInertiaRatio = 0.01;
	std::vector<KeyPoint> keypoints;

	Ptr<SimpleBlobDetector> detector = SimpleBlobDetector::create(params);

	VideoCapture cap(0);

	if (!cap.isOpened())
	{
		cout << "Cannot open the video cam" << endl;
		return -1;
	}

	std::cout << "Hello world!";
	namedWindow("Main");

	while (true)
	{
		// Lees een nieuw frame
		bool bSuccess = cap.read(frame);

		flip(frame, frame, ROTATE_180);

		cvtColor(frame, greyscale, cv::COLOR_RGB2GRAY);

		// Controlleer of het frame goed gelezen is.
		if (!bSuccess)
		{
			cout << "Cannot read a frame from video stream" << endl;
			break;
		}

		//BLOBdetection
		cv::threshold(greyscale, binaryImage, 200, ADAPTIVE_THRESH_GAUSSIAN_C, cv::THRESH_BINARY);

		Mat element = getStructuringElement(MORPH_RECT, Size(1, 1), Point(-1, -1));
		for (int i = 1; i < 10; i++) {
			erode(binaryImage, erosion_dst, element);
			binaryImage = erosion_dst * 255;
		}

		detector->detect(binaryImage, keypoints);

		for (int i = 1; i < 10; i++) {
			dilate(binaryImage, dilation_dst, element);
			binaryImage = dilation_dst * 255;
		}

		Mat blobImg;
		drawKeypoints(frame, keypoints, blobImg, Scalar(0, 0, 255), DrawMatchesFlags::DRAW_RICH_KEYPOINTS);
		putText(blobImg, std::to_string(keypoints.size()), Point(10, 100), FONT_HERSHEY_PLAIN, 10, Scalar(0, 0, 255));

		// Het tonen van beeld
		imshow("Main", blobImg);
		imshow("black", binaryImage);

		//  Wacht 30 ms op ESC-toets. Als ESC-toets is ingedrukt verlaat dan de loop
		if (waitKey(1) == 27)
		{
			cout << "esc key is pressed by user" << endl;
			break;
		}
	}
	return 0;
}

//Depricated
void detectBlobs(string targetWindow)
{
	// cv::threshold(greyscale, binaryImage, 200, ADAPTIVE_THRESH_GAUSSIAN_C, cv::THRESH_BINARY);

	// Mat element = getStructuringElement(MORPH_RECT, Size(2, 2), Point(-1, -1));
	//
	// for (int i = 1; i < 10; i++) {
	// 	erode(binaryImage, erosion_dst, element);
	// 	binaryImage = erosion_dst;
	// }
	//
	// for (int i = 1; i < 10; i++) {
	// 	dilate(binaryImage, dilation_dst, element);
	// 	binaryImage = dilation_dst;
	// }
	//
	// binaryImage.convertTo(binary16S, 3);
	// labelBLOBs(binary16S, labeledImage);
	//
	// std::vector<Point2d*> firstPicVector;
	// std::vector<Point2d*> posVec;
	// std::vector<int> areaVec;

	// nBlobs = labelBLOBsInfo(binary16S, labeledImage, firstPicVector, posVec, areaVec, 1000, 1500);
	//
	// cout << nBlobs << endl;
	//
	// // imshow(targetWindow, binaryImage); //test to show erosed and dilated image
	// show16SImageStretch(labeledImage, targetWindow);
	//
	// binary16S.release();
	// labeledImage.release();
	// binaryImage.release();

	//Ptr<SimpleBlobDetector> detector = SimpleBlobDetector::create(params);

	// Detect blobs
	//detector->detect(frame, keypoints);
}