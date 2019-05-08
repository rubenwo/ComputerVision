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

Mat labeledImage, binary16S, frame, greyscale, binaryImage;

Mat src, erosion_dst, dilation_dst;

int nBlobs;

int main(int argc, char* argv[])
{
	VideoCapture cap(0);

	if (!cap.isOpened())
	{
		cout << "Cannot open the video cam" << endl;
		return -1;
	}

	//double dWidth = cap.get();
	//double dHeight = cap.get(CV_CAP_PROP_FRAME_HEIGHT);

	std::cout << "Hello world!";
	namedWindow("Main");

	while (true)
	{
		// Lees een nieuw frame
		bool bSuccess = cap.read(frame);

		flip(frame, frame, ROTATE_180);

		// Controlleer of het frame goed gelezen is.
		if (!bSuccess)
		{
			cout << "Cannot read a frame from video stream" << endl;
			break;
		}

		//BLOBdetection
		detectBlobs("Main");

		// Het tonen van beeld
		imshow("MyVideo", frame);

		//  Wacht 30 ms op ESC-toets. Als ESC-toets is ingedrukt verlaat dan de loop
		if (waitKey(1) == 27)
		{
			cout << "esc key is pressed by user" << endl;
			break;
		}
	}
	return 0;
}

void detectBlobs(string targetWindow)
{
	cvtColor(frame, greyscale, cv::COLOR_RGB2GRAY);
	cv::threshold(greyscale, binaryImage, 200, ADAPTIVE_THRESH_GAUSSIAN_C, cv::THRESH_BINARY);

	Mat element = getStructuringElement(MORPH_RECT, Size(2, 2), Point(-1, -1));

	for (int i = 1; i < 10; i++) {
		erode(binaryImage, erosion_dst, element);
		binaryImage = erosion_dst;
	}

	for (int i = 1; i < 10; i++) {
		dilate(binaryImage, dilation_dst, element);
		binaryImage = dilation_dst;
	}

	binaryImage.convertTo(binary16S, 3);
	labelBLOBs(binary16S, labeledImage);
	
	std::vector<Point2d*> firstPicVector;
	std::vector<Point2d*> posVec;
	std::vector<int> areaVec;
	
	nBlobs = labelBLOBsInfo(binary16S, labeledImage, firstPicVector, posVec, areaVec, 1000, 1500);
	
	cout << nBlobs << endl;
	
	// imshow("Main", binaryImage); //test to show erosed and dilated image
	show16SImageStretch(labeledImage, targetWindow);
	
	binary16S.release();
	labeledImage.release();
	binaryImage.release();
}