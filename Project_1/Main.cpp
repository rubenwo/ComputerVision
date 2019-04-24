// Functionaliteit: Test- en demoprogramma voor labeling functies
//                  uit blobdetectionavans.h  
// Jan Oostindie, dd 17-3-2016

// Author: Gerben van Dooren, 2124419

#include <opencv2/opencv.hpp>
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <iostream>
#include <string>
#include "BlobDetection.h"

using namespace cv;
using namespace std;

struct Blob
{
	int blobID;
	Point firstPixel;
	Point centre;
	int area;
} blob;

list<Blob> blobs;

///=============================== To do's: =================================///
//struct maken waar je objecten kan opslaan:
//BLOBID
//Firstpixel
//Centre
//Area

//type schermen veranderen zodat ik ze kan resizen.
//zorgen dat de sliders op het scherm vallen
//waardes van de twee sliders uitlezen
//bepalen voor de opgeslagen blobs of deze areas tussen de grenzen vallen
///==========================================================================///

//Trackbar variables:
const int threshhold_slider_max = 255;
int threshhold_sliderMin = 100;
int threshhold_sliderMax = 100;

int threshhold_valueMinArea = 100;
int threshhold_valueMaxArea = 100;

void on_trackbarMin(int, void*)
{
	threshhold_valueMinArea = threshhold_sliderMin;

	//checken welke objecten er nog in beeld mogen komen
	//imshow("")
}

void on_trackbarMax(int, void*)
{
	threshhold_valueMaxArea = threshhold_sliderMax;

	//checken welke objecten er nog in beeld mogen komen

	//update beeld:
	//imshow("")
}


int main(int argc, char* argv[])
{
	// zie: project properties - configuration properties - debugging - command arguments
	if (argc != 2)
	{
		cout << "NB! Geef als command argument volledige padnaam van de imagefile mee" << endl;
		return -1;
	}
	else cout << "De imagefile = " << argv[1] << endl;

	// Lees de afbeelding in
	Mat image;
	image = imread(argv[1], IMREAD_COLOR);
	if (!image.data)
	{
		cout << "Could not open or find the image" << std::endl;
		return -1;
	}

	// De afbeelding converteren naar een grijswaarde afbeelding
	Mat gray_image;
	cvtColor(image, gray_image, COLOR_RGB2GRAY);

	// Converteren naar grijswaarde afbeelding
	cout << "Imagefile: " << argv[1] << " met succes geconverteerd naar grijswaarde beeld." << endl;

	//////////////////////////////////////////////////////////////////////////

	// Grijswaarde afbeelding thresholden
	Mat binaryImage;
	int thresholdvalue = 200;
	threshold(gray_image, binaryImage, thresholdvalue, 1, THRESH_BINARY_INV);
	namedWindow("Original", WINDOW_AUTOSIZE);
	imshow("Original", gray_image);
	waitKey(0);

	// Alvorens bewerkingen uit te voeren op het beeld converteren we deze
	// naar een Mat object met grotere diepte (depth), t.w. 16 bits signed
	Mat binary16S;
	binaryImage.convertTo(binary16S, CV_16S);

	// functie labelBLOBs doet hetzelfde als Label Blobs in VisionLab; input is een
	// binair beeld. Output is een image waarin alle pixels van elke blob voorzien zijn van 
	// volgnummer.
	Mat labeledImage;
	cout << "Total number of BLOBs " << labelBLOBs(binary16S, labeledImage) << endl;

	// functie show16SImageStretch beeld elke image af op 0 - 255 zodat je vrijwel altijd
	// wel een redelijke view krijgt op de data.
	// (show16SImageClip laat alle pixels met een waarde tussen 0 en 255 zien. Waardes onder 0
	// worden op 0 afgebeeld, waardes boven 255 worden op 255 afgebeeld.)
	show16SImageStretch(labeledImage, "Labeled Image");
	resizeWindow("Labeled Image", 600, 600);
	int value = 10;
	createTrackbar("Min Area", "Labeled Image", &value, 1);
	imwrite("c:\\dump\\labeledImage.jpg", labeledImage);

	// labelBLOBsInfo: met deze functie kun je ook BLOBs labelen. De functie geeft van 
	// alle BLOBs de volgende informatie terug:
	// - coordinaten van het eerste pixel van de BLOB
	// - coordinaten van het zwaartepunt van de BLOB.
	// - area van de BLOB
	Mat labeledImage2;
	vector<Point2d *> firstpixelVec2;
	vector<Point2d *> posVec2;
	vector<int> areaVec2;
	labelBLOBsInfo(binary16S, labeledImage2, firstpixelVec2, posVec2, areaVec2);
	show16SImageStretch(labeledImage2, "Labeled Image 2");

	cout << endl << "***************" << endl << endl;

	// Toon alle informatie in de console 
	cout << "Aantal gevonden BLOBs = " << firstpixelVec2.size() << endl;
	for (int i = 0; i < firstpixelVec2.size(); i++)
	{
		cout << "BLOB " << i + 1 << endl;
		cout << "firstpixel = (" << firstpixelVec2[i]->x << "," << firstpixelVec2[i]->y << ")" << endl;
		cout << "centre = (" << posVec2[i]->x << "," << posVec2[i]->y << ")" << endl;
		cout << "area = " << areaVec2[i] << endl;
	}

	cout << endl << "***************" << endl << endl;

	// Met de functie labelBLOBsInfo kun je ook een threshold instellen voor de oppervlakte
	// van de BLOBs.

	Mat labeledImage3;
	vector<Point2d *> firstpixelVec3;
	vector<Point2d *> posVec3;
	vector<int> areaVec3;

	///hier moeten de twee waardes van de slider aan gekoppeld worden
	int minArea = 14000;
	int maxArea = 16000;
	labelBLOBsInfo(binary16S, labeledImage3,
	               firstpixelVec3, posVec3, areaVec3, minArea, maxArea);
	show16SImageStretch(labeledImage3, "Labeled Image 3");

	cout << endl << "***************" << endl << endl;

	// Toon alle informatie in de console 
	cout << "Aantal gevonden BLOBs = " << firstpixelVec3.size() << endl;
	for (int i = 0; i < firstpixelVec3.size(); i++)
	{
		cout << "BLOB " << i + 1 << endl;
		cout << "firstpixel = (" << firstpixelVec3[i]->x << "," << firstpixelVec3[i]->y << ")" << endl;
		cout << "centre = (" << posVec3[i]->x << "," << posVec3[i]->y << ")" << endl;
		cout << "area = " << areaVec3[i] << endl;
	}

	cout << endl << "***************" << endl << endl;

	// Nog een manier om het aantal BLOBs te verkrijgen...
	cout << "Total number of BLOBs = " << maxPixelImage(labeledImage3) << endl;

	string pipo;
	cin >> pipo;

	//Slider initialiseren:
	createTrackbar("Minimum", "Labeled Image", &threshhold_sliderMin, threshhold_slider_max, on_trackbarMin);
	createTrackbar("Maximum", "Labeled Image", &threshhold_sliderMax, threshhold_slider_max, on_trackbarMax);

	//Callback function call for the first image
	on_trackbarMin(threshhold_sliderMin, 0);
	on_trackbarMax(threshhold_sliderMax, 0);

	waitKey(0);
	return 0;
}
