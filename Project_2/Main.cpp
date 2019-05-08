#include <iostream>
#include "../Project_1/BlobDetection.h"
#include <opencv2/opencv.hpp>
#include "opencv2/imgproc/imgproc.hpp" 
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/highgui.hpp"
#include <iostream>

using namespace cv;
using namespace std;

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

	double dWidth = 1920;
	double dHeight = 1080;

	cout << "Frame size : " << dWidth << " x " << dHeight << endl;

	std::cout << "Hello world!";
	namedWindow("Main");

	Mat frame;
	Mat rotatedFrame;

	while (true)
	{
		// Lees een nieuw frame
		bool bSuccess = cap.read(frame);

		flip(frame, frame, 0);

		// Controlleer of het frame goed gelezen is.
		if (!bSuccess)
		{
			cout << "Cannot read a frame from video stream" << endl;
			break;
		}

		// Het tonen van grijswaarde beeld

		rotate(frame, rotatedFrame, ROTATE_180);

		imshow("MyVideo", rotatedFrame);

		//  Wacht 30 ms op ESC-toets. Als ESC-toets is ingedrukt verlaat dan de loop
		if (waitKey(1) == 27)
		{
			cout << "esc key is pressed by user" << endl;
			break;
		}
	}

	return 0;
}