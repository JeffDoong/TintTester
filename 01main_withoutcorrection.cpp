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
    ofstream FileLog;
    FileLog.open("Log.txt", ios::out | ios::app);
//    string Dark = "D.jpg";
//    string Light = "L.jpg";
    FileLog << endl;// << "D Name: " << Dark << " " << "L Name: " << Light << endl;
    Mat L, D, LO, DO, Out, Out2, ROIc, ROI2c;
    double a = getFileCAM(D);
//    double b = getFileIMG(L, Light, CV_LOAD_IMAGE_GRAYSCALE);
    D.copyTo(DO);
//    getFileIMG(DO, Dark, CV_LOAD_IMAGE_COLOR);
//    getFileIMG(LO, Light, CV_LOAD_IMAGE_COLOR);
    if (!a) return 11;
    
    cvtColor(D, D, CV_BGR2GRAY);
    GaussianBlur(D, D, Size(9,9), 2,2);
    vector<Vec3f> circles;
    HoughCircles(D, circles, CV_HOUGH_GRADIENT, 2, D.rows/4, 200, 100);
//    cout << circles.size() << endl;
    Point center(cvRound(circles[0][0]),cvRound(circles[0][1]));
    int radius = cvRound(circles[0][2]);
    circle(D,center,radius,Scalar(0,0,255), 3,8,0);
    Mat ROI(D.size(), D.type(), Scalar::all(0) );
    circle(ROI, center, radius, Scalar::all(255), -1);
    Scalar mean = cv::mean(DO,ROI);
    Mat C(Size(500,500), DO.type(), mean);
    
    cout << "D: " << mean << endl;
    FileLog << "D: " << mean << endl;
    
//    cvtColor(L, L, CV_BGR2GRAY);
//    GaussianBlur(L, L, Size(9,9), 2,2);
//    vector<Vec3f> circles2;
//    HoughCircles(L, circles2, CV_HOUGH_GRADIENT, 2, D.rows/4, 200, 100);
////        cout << circles2.size() << endl;
//    Point center2(cvRound(circles2[0][0]),cvRound(circles2[0][1]));
//    int radius2 = cvRound(circles2[0][2]);
//    circle(L,center2,radius2,Scalar(0,0,255), 3,8,0);
//    Mat ROI2 (L.size(), L.type(), Scalar::all(0) );
//    circle(ROI2, center2, radius2, Scalar::all(255), -1);
//    Scalar mean2 = cv::mean(LO,ROI2);
//    Mat LC(Size(500,500), LO.type(), mean2);
//    
//    cout << "L: " << mean2 << endl;
//    FileLog << "L: " << mean2 << endl;
    
    
    Mat DCalMat = DO;
    Mat LCalMat = LO;
    bitwise_not (ROI, ROIc);
//    bitwise_not (ROI2, ROI2c);
    Scalar DCal = cv::mean(DCalMat, ROIc);
//    Scalar LCal = cv::mean(LCalMat, ROI2c);
    
    imshow("Dcal", DCalMat);
//    imshow("Lcal", LCalMat);
    cout << "Calibration D: " << DCal[2] /*<< " L: " << LCal[2]*/ << endl;
    FileLog << "Calibration D: " << DCal[2]/* << " L: " << LCal[2] */<< endl;


//    imshow("ROI", ROI);
    DO.copyTo(Out, ROI);
    imshow("D", Out);
    imshow("D Color", C);
//    LO.copyTo(Out2, ROI2);
//    imshow("L", Out2);
//    imshow("L Color", LC);
    waitKey(500);
    return 0;
}
