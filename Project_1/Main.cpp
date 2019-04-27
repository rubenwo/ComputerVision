#include <iostream>
#include <opencv2/core/mat.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui/highgui.hpp>
#include "opencv2/imgproc/imgproc.hpp"
#include "BlobDetection.h"


cv::Mat image;
cv::Mat binaryImage;
cv::Mat labeledImage;

void update(const char* window, cv::Mat* img)
{
	std::cout << "Update window: " << window << std::endl;
	cv::imshow(window, *img);
}

int min_value;

void on_trackbar_min(int pos, void* userData)
{
	std::cout << "Min_Value: " << min_value << std::endl;
	show16SImageStretch(labeledImage, "Area");
}

int max_value;

void on_trackbar_max(int pos, void* userData)
{
	std::cout << "Blobs: " << std::endl;
	show16SImageStretch(labeledImage, "Area");
}

int threshold_value = 215;

void on_threshold_trackbar(int pos, void* userData)
{
	cv::threshold(image, binaryImage, threshold_value, 1, cv::THRESH_BINARY_INV);

	Mat binary16S;

	binaryImage.convertTo(binary16S, CV_16S);

	int nBlobs = labelBLOBs(binary16S, labeledImage);
	std::cout << "Blob amount: " << nBlobs << std::endl;
	update("Threshold", &binaryImage);
}

int main(int argc, char* argv[])
{
	if (argc != 2)
	{
		std::cout << "NB! Geef als command argument volledige padnaam van de imagefile mee" << std::endl;
		return -1;
	}
	else std::cout << "De imagefile = " << argv[1] << std::endl;

	// Load 
	image = cv::imread(argv[1], cv::IMREAD_GRAYSCALE);


	cv::namedWindow("Grayscale");
	cv::imshow("Grayscale", image);
	//cv::waitKey(0);

	cv::namedWindow("Threshold");
	binaryImage = cv::Mat();
	cv::imshow("Threshold", image);
	cv::createTrackbar("Threshhold_Slider", "Threshold", &threshold_value, 255, on_threshold_trackbar);
	on_threshold_trackbar(1,NULL);
	cv::waitKey(0);

	cv::namedWindow("Area");
	cv::imshow("Area", labeledImage);
	cv::createTrackbar("Min_Value_Slider", "Area", &min_value, 20000, on_trackbar_min);
	cv::createTrackbar("Max_Value_Slider", "Area", &max_value, 20000, on_trackbar_max);

	cv::waitKey(0);
}
