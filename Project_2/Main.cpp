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

Mat labeledImage;
Mat frame;

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
	Mat greyscale, binaryImage;
	cvtColor(frame, greyscale, cv::COLOR_RGB2GRAY);
	cv::threshold(greyscale, binaryImage, 128, ADAPTIVE_THRESH_GAUSSIAN_C, cv::THRESH_BINARY);

	//BLOBdetection
	Mat binary16S;
	binaryImage.convertTo(binary16S, 3);
	labelBLOBs(binary16S, labeledImage);
	

	std::vector<Point2d*> firstPicVector;
	std::vector<Point2d*> posVec;
	std::vector<int> areaVec;

	int nBlobs = labelBLOBsInfo(binary16S, labeledImage, firstPicVector, posVec, areaVec, 500, 700);

	cout << nBlobs << endl;

	show16SImageStretch(labeledImage, targetWindow);
	binary16S.release();
	labeledImage.release();
}