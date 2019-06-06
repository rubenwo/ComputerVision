#include "FingerCounter.hpp"
#include <opencv2/imgproc.hpp>
#include <corecrt_math_defines.h>

std::vector<std::vector<cv::Point>> contours;
std::vector<cv::Point> contourPoints;
std::vector<cv::Vec4i> hierarchy;

int lrgctridx = 0;
int maxarea = 0;

FingerCounter::FingerCounter()
{
}

int GetAngleABC(cv::Point a, cv::Point b, cv::Point c)
{
	cv::Point ab = { b.x - a.x, b.y - a.y };
	cv::Point cb = { b.x - c.x, b.y - c.y };

	float dot = (ab.x * cb.x + ab.y * cb.y); // dot product
	float cross = (ab.x * cb.y - ab.y * cb.x); // cross product

	float alpha = atan2(cross, dot);

	return (int)floor(alpha * 180 / M_PI + 0.5);
}

cv::UMat FingerCounter::findFingersCount(cv::UMat input_image, cv::UMat frame)
{
	contours.clear();
	hierarchy.clear();
	int fingers = 0;

	cv::findContours(input_image, contours, hierarchy, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE);

	for (int i = 0; i < contours.size(); i++)
	{
		double a = cv::contourArea(contours[i]);
		if (a > maxarea)
		{
			maxarea = a;
			lrgctridx = i;
		}
	}

	std::vector<std::vector<int> >inthull(contours.size());
	std::vector<std::vector<cv::Vec4i>> defects(contours.size());
	for (int i = 0; i < contours.size(); i++) {
		cv::convexHull(contours[i], inthull[i]);
		if (inthull[i].size() > 3)
			convexityDefects(contours[i], inthull[i], defects[i]);
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

	return frame;
}
