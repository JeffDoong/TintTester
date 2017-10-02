//
//  main.cpp
//  Angle_Test
//
//  Created by Russell Ohnemus on 7/7/15.
//  Copyright (c) 2015 Russell Ohnemus. All rights reserved.
//

#include <iostream>
#include <fstream>
#include <cmath>
#include <time.h>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/core/types_c.h>

#include "help.h"
#include "cvmatchtemp.h"

using namespace std;
using namespace cv;

int main() {
    VideoCapture cap(0);
    if (!cap.isOpened()) return 13;
    const int Capacity = 1;
    ofstream FileLog;
    FileLog.open("Log.txt", ios::out | ios::app);
    FileLog << endl;
    Mat IN, HOUGH, COLOR, ROI, black, Out, Display, Wait, Error, Start;
    int radius[Capacity];
    vector<Point> center (Capacity);
    Scalar CalibrationValue, mean[Capacity];
    bool good = false;
    bool more;

    cvNamedWindow("Display", CV_WINDOW_NORMAL);
    cvSetWindowProperty("Display", CV_WND_PROP_FULLSCREEN, CV_WINDOW_FULLSCREEN);
    //moveWindow("Display", 0,0);
    getFileIMG(Wait, "Wait.jpg", CV_LOAD_IMAGE_COLOR);
    getFileIMG(Error, "Error.png", CV_LOAD_IMAGE_COLOR);    
    getFileIMG(Start, "Start.png", CV_LOAD_IMAGE_COLOR);
    imshow("Display", Start);
    waitKey(0);
    do {
	imshow("Display", Wait);
	waitKey(10);
	cout << "wait" << endl;
        //double a = getFileCAM(IN);
        //if (!a) return 11;
        for (int i = 0; i < 100; i++) cap >> IN;
	IN.copyTo(COLOR);
        
        cvtColor(IN, HOUGH, CV_BGR2GRAY);
        GaussianBlur(HOUGH, HOUGH, Size(9,9), 2,2);
        vector<Vec3f> circles (4);
        HoughCircles(HOUGH, circles, CV_HOUGH_GRADIENT, 2, HOUGH.rows/4, 200, 100);
        
        double j;
        if (circles.size() == 0) {
            j = 0;
            cerr << "Warning: No Circles Found" << endl;
        }
        else if (circles.size() < Capacity)  j = circles.size();
        else j = Capacity;
        
        if (j) {
        for (int i = 0; i < j; i++) {
    
            center[i] = Point(cvRound(circles[i][0]),cvRound(circles[i][1]));
            radius[i] = cvRound(circles[i][2]);
            Mat ROI(COLOR.size(), CV_8U, Scalar::all(0) );
            circle(ROI, center[i], radius[i], Scalar::all(255), -1);
            mean[i] = cv::mean(COLOR,ROI);
            Mat ColorChip(Size(480,320), COLOR.type(), mean[i]);
            ColorChip.copyTo(Display);
            imshow("Display", ColorChip);
            cout << "Chip" << i << "RGB: " << mean[i] << endl;
            FileLog << "C" << i << "RGB: " << mean[i] << endl;
  
        }

        Mat CAL(COLOR.size(), CV_8U, Scalar::all(255));
        for (int i = 0; i < Capacity; i++) {
            circle(CAL, center[i], radius[i], Scalar::all(0), -1);
        }
        CalibrationValue = cv::mean(COLOR, CAL);
        
        double hsV = max(mean[0][0]/2.55, max(mean[0][1]/2.55, mean[0][2]/2.55));
        double hsVCal = max(CalibrationValue[0]/2.55, max(CalibrationValue[2]/2.55, CalibrationValue[2]/2.55));
        
        double val = hsV;
        double cal = hsVCal;
        int num = cvRound(val + 100 - cal);
        if (num >= 62) good = true;
	else good = false;
        
        cout << "Calibration A: " << cal  << endl;
        FileLog << "Calibration A: " << cal << endl;
        cout << "Corrected Value A:  " << num << endl;
        FileLog << "Corrected Value A:  " << num << endl;
        putText(Display, "Chip Value:              " + tostring(val), Point(20,40), FONT_HERSHEY_DUPLEX, .75, Scalar::all(255),1.5);
        putText(Display, "Callibration Value:       " + tostring(cal), Point(20,70), FONT_HERSHEY_DUPLEX, .75, Scalar::all(255),1.5);
        putText(Display, tostring(num), Point(145,200), FONT_HERSHEY_DUPLEX, 4.5, Scalar::all(255), 6);
        
	if (!circles.size()) Error.copyTo(Display);
        else if (good) {
            putText(Display, "PASS", Point(100,282), FONT_HERSHEY_TRIPLEX, 2, Scalar::all(255));
            circle(Display, Point(340,260), 25, Scalar(0,255,0), -1);
            cout << "GOOD" << endl;
        }
        else {
            putText(Display, "FAIL", Point(100,282), FONT_HERSHEY_TRIPLEX, 2, Scalar::all(255));
            circle(Display, Point(340,260), 25, Scalar(0,0,255), -1);
            cout << "BAD" << endl;
        }
        imshow("Display", Display);
        }
	else imshow("Display", Error);
	more = true;
	if  (waitKey(0) == 27) more = false;

    } while (more);
    
    return 0;
}

