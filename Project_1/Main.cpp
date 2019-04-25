#include <iostream>
#include <opencv2/core/mat.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui/highgui.hpp>

void update()
{
}

int min_value;

void on_trackbar_min(int pos, void* userData)
{
}

int max_value;

void on_trackbar_max(int pos, void* userData)
{
}

int threshold;

void on_threshold_trackbar(int pos, void* userData)
{
	std::cout << "Treshhold " << threshold << std::endl;
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
	cv::Mat image = cv::imread(argv[1], cv::IMREAD_GRAYSCALE);


	cv::namedWindow("Grayscale");
	cv::imshow("Grayscale", image);
	cv::waitKey(0);

	cv::namedWindow("Threshold");
	cv::imshow("Threshold", image);
	cv::createTrackbar("Threshhold_Slider", "Threshold", &threshold, 255, on_threshold_trackbar);

	cv::waitKey(0);
}
