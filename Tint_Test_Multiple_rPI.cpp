//
//  main.cpp
//  Tint_Test
//
//  Created by Russell Ohnemus on 7/7/15.
//  Edited by Russell Ohnemus and Art Cai 8/2016
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

/* load up the pins as inputs so they read a high signal when pressed */

	wiringPiSetup(); // Initialize WiringPi Using Default WiringPi Pin Numbers.
	pinMode(0, OUTPUT); // Pin 0 controls LED in Run button.
	pinMode(3, INPUT); // Pin 3 receives input from Run button.
	pinMode(4, INPUT);
	pinMode(5,INPUT);
	digitalRead(4);
	digitalRead(5);
	// printf("State of 4: %d\n", digitalRead(4));
	// printf("State of 5: %d\n", digitalRead(5));
	pullUpDnControl(4, PUD_DOWN);
	pullUpDnControl(5, PUD_DOWN);
	pullUpDnControl(3, PUD_DOWN);
	
	int prodNum = 6;  // how many items you have minus 1 due to indexing (arrays start counting from 0)
	

	//    ----------------------------------------------- INITIALIZATION ----------------------------------------------------
	{
		int h;
		for (h = 0; h < 2; h++)
			printf("\n\n\n\n\n\n\n\n\n\n");
	
	}
	
	
	ifstream read("settings.txt"); // opens settings.txt to read tint values
	int productBounds[21];  //how many lines to read
	int upperLim, lowerLim, offset;
	int n;
	int i = 0;

	while (read >> n) {
		productBounds[i] = n;
		i++;
	}
	read.close();
	
	ifstream index("index.txt"); //opens index.txt where to read the last product number 
	int pCycle = 0; 
	index >> pCycle; //starting item number from index to pCycle...pCycle is the integer for product number
	string pArray[] = { "Grinder", "12 Cup Coffee Maker", "9 Cup Coffee Maker", "Imersion Blender", "Hand Mixer", "Kettel", "Adjustable Kettel" }; //name of all the items
	

	if(0 || (digitalRead(4) && digitalRead(5))){ //only active loop if pins 4 and 5 are high
	
	cvNamedWindow("Display", CV_WINDOW_NORMAL); // Create window for GUI output.
	cvSetWindowProperty("Display", CV_WND_PROP_FULLSCREEN, CV_WINDOW_FULLSCREEN);
	

Mat Display(Size(480, 320), CV_8U, Scalar::all(0));
Mat Save = Display;
	imshow("Display", Display);
	putText(Display, "LOADING...", Point(60, 300), CV_FONT_HERSHEY_SIMPLEX, .5, Scalar::all(150), 1);
	

	for (int i = 0; i < 45; i++) { //loading function that appears below the "LOADING..."
		waitKey(10);
		rectangle(Display, Point(60, 310), Point(60 + 8 * i, 315), Scalar::all(150));

		imshow("Display", Display);
		
		}
	

	Display = Save;

		cout << pArray[pCycle] << endl;
		usleep(500000);
		string REV, FOR, SEL, REV2, FOR2;

// the following loops cycle through pCycle numbering and matches each item number to the item in pArray... starts at 0 and goes to 6 for 7 items. the initial loop and display is based on the previous run (seen in index)

		if(pCycle == prodNum) {  //if the pCycle number is at the last product, display the two previous items and the two first items of pArray	
			REV = pArray[pCycle - 1];
			FOR = pArray[0];
			REV2 = pArray[pCycle - 2];
			FOR2 = pArray[1];
			}
		else if(pCycle == 0) { //if pCycle is at the first item of pArray, show the next two and the last two items
			REV = pArray[prodNum];
			FOR = pArray[pCycle + 1];
			REV2 = pArray[prodNum - 1];
			FOR2 = pArray[pCycle +2];
			}
		else if(pCycle == (prodNum -1)){ //if pCycle is at the second to last item, display two previous, one after, and the first item
			REV = pArray[prodNum -2];
			FOR = pArray[pCycle + 1];
			REV2 = pArray[prodNum - 3];
			FOR2 = pArray[0];
			}
		else if(pCycle == 1){ //if pCycle is at the second item, show two after, one before, and the last item
			REV = pArray[0];
			FOR = pArray[pCycle + 1];
			REV2 = pArray[prodNum];
			FOR2 = pArray[pCycle + 2];
			}
		else { //everyother pCycle number, display 2 before, 2 after.
			REV = pArray[pCycle - 1];
			FOR = pArray[pCycle + 1];
			REV2 = pArray[pCycle - 2];
			FOR2 = pArray[pCycle + 2];
			}

		SEL = pArray[pCycle]; //this is the item to select

// next lines are the up, down, and circle graphics seen on the side of the selection display
		// up triangle, center circle, down triangle
		circle(Display, Point(30, 150), 8, Scalar(150, 250, 50), -1,1);
		circle(Display, Point(30,150), 5, Scalar(0,0,0), -1,1); // inside circle
		line(Display, Point(23, 50), Point(37,50), Scalar(150,250,50), 1, 4,0); // bottom line of triangle
		line(Display, Point(30,35), Point(23,50), Scalar(150,250,50), 1, 8, 0);
		line(Display, Point(30,35), Point(37,50), Scalar(150,250,50), 1, 8, 0);

		line(Display, Point(23, 240), Point(37, 240), Scalar(150,250,50), 1, 4,0); // top line of triangle
		line(Display, Point(30, 255), Point(23,240), Scalar(150,250,50), 1, 8, 0);
		line(Display, Point(30, 255), Point(37, 240), Scalar(150,250,50), 1, 8, 0);
		//triangle fill 
		line(Display, Point(30,36), Point(25,49), Scalar(150,250,50), 2, 8, 0);
		line(Display, Point(30,36), Point(35,49), Scalar(150,250,50), 2, 8, 0);
		line(Display, Point(24, 49), Point(36, 49), Scalar(150,250,50),2,8,0);
		line(Display, Point(30,35), Point(30,49), Scalar(150,250, 50), 2, 8, 0);
		line(Display, Point(28, 45), Point(28, 48), Scalar(150,250,50),2,8,0);
		line(Display, Point(32, 45), Point(32, 48), Scalar(150,250,50),2,8,0); 

		line(Display, Point(30,254), Point(25,241), Scalar(150,250,50), 2, 8, 0);
		line(Display, Point(30,254), Point(35,241), Scalar(150,250,50), 2, 8, 0);
		line(Display, Point(24, 241), Point(36, 241), Scalar(150,250,50),2,8,0);
		line(Display, Point(30,254), Point(30,241), Scalar(150,250, 50), 2, 8, 0);
		line(Display, Point(28, 250), Point(28, 242), Scalar(150,250,50),2,8,0);
		line(Display, Point(32, 250), Point(32, 242), Scalar(150,250,50),2,8,0);

//these lines display the selection options
		putText(Display, REV2, Point(60, 50), CV_FONT_HERSHEY_SIMPLEX, .5, Scalar::all(200), 1);
		putText(Display, REV, Point(60, 100), CV_FONT_HERSHEY_SIMPLEX, .5, Scalar::all(200), 1);
		putText(Display, SEL, Point(60, 160), CV_FONT_HERSHEY_SIMPLEX, 1, Scalar::all(200), 2);
	 	putText(Display, FOR, Point(60, 200), CV_FONT_HERSHEY_SIMPLEX, .5, Scalar::all(200), 1);
		putText(Display, FOR2, Point(60, 250), CV_FONT_HERSHEY_SIMPLEX, .5, Scalar::all(200), 1);
		
		imshow("Display", Display);
		waitKey(1);
		
		while (1){ //each pin related to its function
			int NEXT = digitalRead(4);
			int BACK = digitalRead(5);
			int EXIT = digitalRead(3);
			// cout <<"hellppppmeeeee" << endl;
			usleep(200000);
	
			if(EXIT == 1){ break; } //if the run is pressed, saves the product number and goes to normal testing mode
			else if( BACK == 0 && NEXT == 0){ continue; } //if nothing is pressed continue to loop and not scroll through
			else if(BACK == 1 && NEXT == 1){ continue; } //if both are pressed continue to loop and not scroll through 

			else if(NEXT == 1){ //if pin 4 is high, go to next item
				pCycle++;
				// cout << pCycle << endl;
				
				if(pCycle >= (prodNum + 1)){ pCycle = 0; } //if pCycle is at the last item, go back to firs item
				// cout << pCycle << endl;
				cout << pCycle << "   " << pArray[pCycle] << endl;
			}
	
			else if(BACK == 1){ //if pin 5 is high, go backwards one item
				pCycle--;
				if(pCycle < 0){ pCycle = 6; } //if pCycle is "before" the first item, go to the last item

				cout << pCycle << "   " << pArray[pCycle] << endl;
				}
		//Below is the same indexing as above just outside of the initial selection display which was based on the previous run.
			if(pCycle == prodNum) {  	
				REV = pArray[pCycle - 1];
				FOR = pArray[0];
				REV2 = pArray[pCycle - 2];
				FOR2 = pArray[1];
				}
			else if(pCycle == 0) {
				REV = pArray[prodNum];
				FOR = pArray[pCycle + 1];
				REV2 = pArray[prodNum - 1];
				FOR2 = pArray[pCycle +2];
				}
			else if(pCycle == (prodNum -1)){
				REV = pArray[prodNum - 2];
				FOR = pArray[pCycle + 1];
				REV2 = pArray[prodNum - 3];
				FOR2 = pArray[0];
				}
			else if(pCycle == 1){
				REV = pArray[0];
				FOR = pArray[pCycle + 1];
				REV2 = pArray[prodNum];
				FOR2 = pArray[pCycle + 2];
				}
			else {
				REV = pArray[pCycle - 1];
				FOR = pArray[pCycle + 1];
				REV2 = pArray[pCycle - 2];
				FOR2 = pArray[pCycle + 2];
				}

			SEL = pArray[pCycle];

//below is the same up, down, selection graphics as above
			Mat Display(Size(480, 320), CV_8U, Scalar::all(0));
			circle(Display, Point(30, 150), 8, Scalar(150, 250, 50), -1,1);
			circle(Display, Point(30,150), 5, Scalar(0,0,0), -1,1);	
			line(Display, Point(23, 50), Point(37,50), Scalar(150,250,50), 1, 4,0); // bottom line of triangle
			line(Display, Point(30,35), Point(23,50), Scalar(150,250,50), 1, 8, 0);
			line(Display, Point(30,35), Point(37,50), Scalar(150,250,50), 1, 8, 0);
			line(Display, Point(23, 240), Point(37, 240), Scalar(150,250,50), 1, 4,0); // top line of triangle
			line(Display, Point(30, 255), Point(23,240), Scalar(150,250,50), 1, 8, 0);
			line(Display, Point(30, 255), Point(37, 240), Scalar(150,250,50), 1, 8, 0);
//triangle fills
			line(Display, Point(30,36), Point(25,49), Scalar(150,250,50), 2, 8, 0);
			line(Display, Point(30,36), Point(35,49), Scalar(150,250,50), 2, 8, 0);
			line(Display, Point(24, 49), Point(36, 49), Scalar(150,250,50),2,8,0);
			line(Display, Point(30,35), Point(30,49), Scalar(150,250, 50), 2, 8, 0);
			line(Display, Point(28, 45), Point(28, 48), Scalar(150,250,50),2,8,0);
			line(Display, Point(32, 45), Point(32, 48), Scalar(150,250,50),2,8,0); 
	
			line(Display, Point(30,254), Point(25,241), Scalar(150,250,50), 2, 8, 0);
			line(Display, Point(30,254), Point(35,241), Scalar(150,250,50), 2, 8, 0);
			line(Display, Point(24, 241), Point(36, 241), Scalar(150,250,50),2,8,0);
			line(Display, Point(30,254), Point(30,241), Scalar(150,250, 50), 2, 8, 0);
			line(Display, Point(28, 250), Point(28, 242), Scalar(150,250,50),2,8,0);
			line(Display, Point(32, 250), Point(32, 242), Scalar(150,250,50),2,8,0);


			putText(Display, REV2, Point(60, 50), CV_FONT_HERSHEY_SIMPLEX, .5, Scalar::all(200), 1);
			putText(Display, REV, Point(60, 100), CV_FONT_HERSHEY_SIMPLEX, .5, Scalar::all(200), 1);
			putText(Display, SEL, Point(60, 160), CV_FONT_HERSHEY_SIMPLEX, 1, Scalar::all(200), 2);
			putText(Display, FOR, Point(60, 200), CV_FONT_HERSHEY_SIMPLEX, .5, Scalar::all(200), 1);
			putText(Display, FOR2, Point(60, 250), CV_FONT_HERSHEY_SIMPLEX, .5, Scalar::all(200), 1);
			imshow("Display", Display);
			waitKey(1);
			// cout << pArray[pCycle] << endl;
		}
		
		ofstream WriteSettings;
		WriteSettings.open("index.txt"); //writes the chosen option into index for the next run
		WriteSettings << pCycle << endl;
		WriteSettings.close();

	}

//tint bounds put into productBound array
	upperLim = productBounds[(pCycle ) * 3 ];
	lowerLim = productBounds[(pCycle) * 3 + 1];
	offset = productBounds[(pCycle) * 3 + 2];

	 cout << upperLim << endl << lowerLim << endl << offset;


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

	wiringPiSetup(); // Initialize WiringPi Using Default WiringPi Pin Numbers.
	pinMode(0, OUTPUT); // Pin 0 controls LED in Run button.
	pinMode(3, INPUT); // Pin 3 receives input from Run button.
	pinMode(4, INPUT);
	pinMode(5,INPUT);


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

	Mat Screen(Size(480, 320), CV_8U, Scalar::all(0));
	imshow("Display", Screen);
	putText(Screen, "LOADING...", Point(90, 160), CV_FONT_HERSHEY_SIMPLEX, 2, Scalar::all(150), 2);
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
		val = max(MeanValue[0] / 2.55, max(MeanValue[1] / 2.55, MeanValue[2] / 2.55)); // Convert BGR Values to HSV V Value. This value is the chip value
		cal = max(CalibrationValue[0] / 2.55, max(CalibrationValue[2] / 2.55, CalibrationValue[2] / 2.55)); //value of color around chip 
		num = cvRound(val + 100 - cal + SHIFT); //number of val and cal converted to arbitrary value used to be compared to values in "settings.txt"
		NumString = "Chip: " + tostring(val) + " Cal.: " + tostring(cal) + " Tint Value: " + tostring(num);

		if (1) {
			cout << "Debug: " << MeanValue << " " << CalibrationValue << endl;
			cout << "Debug: val=" << val << " cal=" << cal << " num=" << num << endl;
			cout << "Debug: " << BOUNDLOW << " " << num << " " << BOUNDHIGH << endl;
		}

		if (num >= BOUNDLOW && num <= BOUNDHIGH) good = true; // Determine Pass or Fail.
		else good = false;

//tests to see if tint is within bounds, displays respective readings

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

			int loc = 150 + (num - BOUNDLOW)*(180 / (BOUNDHIGH - BOUNDLOW)); //converts num to pixel location
			
			if (loc < 6) { //if the pixel location goes off below 6 in x-dir, put circle at the beginning of the black and white color gradient seen at the top of run display
				circle(Display, Point(6, 15), 7, Scalar::all(200), -1);
				circle(Display, Point(6, 15), 6, Scalar::all(0), -1);
				}
			else if(loc > 474) { //if the pixel location goes above 474 in x-dir, put circle at end of black and white color gradient seen at the top of run display
				circle(Display, Point(474, 15), 7, Scalar::all(200), -1);
				circle(Display, Point(474, 15), 6, Scalar::all(0), -1);
				}
			else { //for all other values outside of bounds, put where the conversion sets them
				circle(Display, Point(loc, 15), 7, Scalar::all(200), -1);
				circle(Display, Point(loc, 15), 6, Scalar::all(0), -1);
			}

			FileLog << endl << getTime() << " Failed: " << NumString;
		}


		putText(Display, NumString, Point(12, 310), FONT_HERSHEY_SIMPLEX, .3, Scalar::all(255)); //displays chip value, cal value (around chip), and number conversion
		putText(Display, ErrString, Point(100, 285), FONT_HERSHEY_SIMPLEX, .7, Scalar::all(255)); //displays no chip detected
		putText(Display, pArray[pCycle], Point(360, 310), FONT_HERSHEY_SIMPLEX, .3, Scalar::all(255)); //shows which item tested..also which item that will be saved to index file
		
		
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

