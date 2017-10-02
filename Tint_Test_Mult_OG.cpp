//
//  main.cpp
//  Tint_Test
//
//  Created by Russell Ohnemus on 7/7/15.
//  Copyright (c) 2015 Russell Ohnemus. All rights reserved.
//  Icons made by Freepik from www.flaticon.com 
//

#include <iostream>
#include <fstream>
#include <cmath>
#include <time.h>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/core/types_c.h>

#include "help.h"
#include "wiringPi.h"
#include "cvmatchtemp.h"

using namespace std;
using namespace cv;

int main() {

	//    ----------------------------------------------- INITIALIZATION ----------------------------------------------------	{
	{	int h;
		for (h = 0; h < 2; h++)
			printf("\n\n\n\n\n\n\n\n\n\n");
	}
	cout << "Press 'C' to Select Product Type     Press 'S' to Start \n";
	char pInput = 'a';
	int pCycle = 0;
	string pArray[] = { "Grinder", "12 Cup Coffee Maker", "9 Cup Coffee Maker", "Imersion Blender", "Hand Mixer", "Kettel", "Adjustable Temperature Kettel" };

	while (pInput != 's') {

		cin >> pInput;
		if (pInput == 'c') {
			if (pCycle >= 7) {
				pCycle = 0;
			}
			else if (pCycle <= 6) {
				pCycle++;
			}
			{
				int h;
				for (h = 0; h < 2; h++)
					printf("\n\n\n\n\n\n\n\n\n\n");
			}
			cout << pArray[pCycle] + "\n";
		}
		else if (pInput == 's') {
			break;
		}

		else {
			cout << "Invalid Input \n";
		}
	}



	int pType;
	pType = pCycle;

	ifstream read("settings.txt");
	int productBounds[21];
	int upperLim, lowerLim, offset;
	int n;
	int i = 0;

	while (read >> n) {
		productBounds[i] = n;
		i++;
	}
	upperLim = productBounds[(pType - 1) * 3];
	lowerLim = productBounds[(pType - 1) * 3 + 1];
	offset = productBounds[(pType - 1) * 3 + 2];

	cout << upperLim << endl << lowerLim << endl << offset;

	getchar();




	int BOUNDLOW = lowerLim; // <------------------------ Adjust Pass/Fail Bounds
	int BOUNDHIGH = upperLim;
	int SHIFT = offset;

	//<---------------------------------------------------------------- Start Image Processing

	VideoCapture cap(0); // Camera Stream
	if (!cap.isOpened()) return 13;
	Mat IN, Display, Start, Wait, ROI, CalROI, Pass, Fail, Error;
	Scalar MeanValue, CalibrationValue;
	int num, key;
	double val, cal;
	bool good = false, more, debug = false;
	string ErrString, NumString;

	ofstream FileLog;
	FileLog.open("DataLog.txt", ios::out | ios::app);
	FileLog << endl << "\nTesting started: " << getTime();

/*	wiringPiSetup(); // Initialize WiringPi Using Default WiringPi Pin Numbers.
	pinMode(0, OUTPUT); // Pin 0 controls LED in Run button.
	pinMode(3, INPUT); // Pin 3 receives input from Run button.
	pinMode(4, INPUT);
	pinMode(5,INPUT);
*/

	cvNamedWindow("Display", CV_WINDOW_NORMAL); // Create window for GUI output.
	cvSetWindowProperty("Display", CV_WND_PROP_FULLSCREEN, CV_WINDOW_FULLSCREEN); // Force window fullscreen.

	getFileIMG(Start, "Start.png", CV_LOAD_IMAGE_COLOR);
	getFileIMG(Wait, "Wait.png", CV_LOAD_IMAGE_COLOR); // Load Images.
	getFileIMG(ROI, "ROI.png", CV_LOAD_IMAGE_GRAYSCALE);
	getFileIMG(CalROI, "CalROI.png", CV_LOAD_IMAGE_GRAYSCALE);
	getFileIMG(Pass, "Pass.png", CV_LOAD_IMAGE_COLOR);
	getFileIMG(Fail, "Fail.png", CV_LOAD_IMAGE_COLOR);
	getFileIMG(Error, "Error.png", CV_LOAD_IMAGE_COLOR);

	cvtColor(ROI, ROI, CV_BGR2GRAY);
	cvtColor(CalROI, CalROI, CV_BGR2GRAY);
	ROI.convertTo(ROI, CV_8U);
	CalROI.convertTo(CalROI, CV_8U);

	//    ------------------------- DISPLAY LOADING SCREEN TO ALLOW WINDOW TO FIT TO SCREEN ---------------------------------

	Mat Screen(Size(480, 320), CV_8U, Scalar::all(50));
	imshow("Display", Screen);
	putText(Screen, "Loading...", Point(90, 160), CV_FONT_HERSHEY_SIMPLEX, 2, Scalar::all(150), 2);
	for (int i = 0; i < 45; i++) {
		waitKey(10);
		rectangle(Screen, Point(60, 255), Point(60 + 8 * i, 265), Scalar::all(150));
		imshow("Display", Screen);
	}
	waitKey(100);
	imshow("Display", Start);
	waitKey(100);
	digitalWrite(0, HIGH);
	waitButton(3, 0);
	digitalWrite(0, LOW);

	//    ------------------------------------------------ START TESTING LOOP ------------------------------------------------

	do {
		imshow("Display", Wait);
		waitKey(5);
		for (int i = 0; i < 100; i++) cap >> IN; // Captures multiple images to allow camera properly expose.

		MeanValue = cv::mean(IN, ROI);
		CalibrationValue = cv::mean(IN, CalROI);
		val = max(MeanValue[0] / 2.55, max(MeanValue[1] / 2.55, MeanValue[2] / 2.55)); // Convert BGR Values to HSV V Value
		cal = max(CalibrationValue[0] / 2.55, max(CalibrationValue[2] / 2.55, CalibrationValue[2] / 2.55));
		num = cvRound(val + 100 - cal + SHIFT);
		NumString = "Chip: " + tostring(val) + " Cal.: " + tostring(cal) + " Tint Value: " + tostring(num);

		if (debug) {
			cout << "Debug: " << MeanValue << " " << CalibrationValue << endl;
			cout << "Debug: val=" << val << " cal=" << cal << " num=" << num << endl;
			cout << "Debug: " << BOUNDLOW << " " << num << " " << BOUNDHIGH << endl;
		}

		if (num >= BOUNDLOW && num <= BOUNDHIGH) good = true; // Determine Pass or Fail.
		else good = false;

		if (abs(val - cal) < 10) { // Display Output.
			Error.copyTo(Display);
			ErrString = "Error: No Chip Detected";
			cout << "ERROR" << endl;
			FileLog << endl << getTime() << " " << ErrString;
		}
		else if (good) {
			Pass.copyTo(Display);
			ErrString = " ";
			cout << "GOOD" << endl;
			FileLog << endl << getTime() << " Passed: " << NumString;
		}
		else {
			Fail.copyTo(Display);
			ErrString = " ";
			cout << "BAD" << endl;
			circle(Display, Point(150 + (num - BOUNDLOW)*(180 / (BOUNDHIGH - BOUNDLOW)), 15), 7, Scalar::all(200), -1);
			circle(Display, Point(150 + (num - BOUNDLOW)*(180 / (BOUNDHIGH - BOUNDLOW)), 15), 6, Scalar::all(0), -1);
			FileLog << endl << getTime() << " Failed: " << NumString;
		}

		if (num < BOUNDLOW - (BOUNDHIGH - BOUNDLOW)) {
			circle(Display, Point(15, 15), 7, Scalar::all(200), -1);
			circle(Display, Point(15, 15), 6, Scalar::all(0), -1);
		}
		else if (num > BOUNDHIGH + (BOUNDHIGH - BOUNDLOW) && num < 100 + SHIFT - 1) {
			circle(Display, Point(465, 15), 7, Scalar::all(200), -1);
			circle(Display, Point(465, 15), 6, Scalar::all(0), -1);
		}
		else {
			circle(Display, Point(150 + (num - BOUNDLOW)*(180 / (BOUNDHIGH - BOUNDLOW)), 15), 7, Scalar::all(200), -1);
			circle(Display, Point(150 + (num - BOUNDLOW)*(180 / (BOUNDHIGH - BOUNDLOW)), 15), 6, Scalar::all(0), -1);
		}
		putText(Display, NumString, Point(12, 310), FONT_HERSHEY_SIMPLEX, .3, Scalar::all(255));
		putText(Display, ErrString, Point(100, 285), FONT_HERSHEY_SIMPLEX, .7, Scalar::all(255));
		imshow("Display", Display);

		more = true; // Continue?
		debug = false;
		digitalWrite(0, HIGH);
		waitKey(100);
		waitButton(3, 0);
		key = waitKey(10);
		if (key == 27) more = false;
		else if (key == 'd') debug = true;
		digitalWrite(0, LOW);

	} while (more);
	return 0;
}

