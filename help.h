//
//  help.h
//  Initial Exploration
//
//  Created by Russell Ohnemus on 6/18/15.
//  Copyright (c) 2015 Russell Ohnemus. All rights reserved.
//

#ifndef __Initial_Exploration__help__
#define __Initial_Exploration__help__

#include <stdio.h>
#include <unistd.h>
#include <iostream>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui/highgui.hpp>

using namespace cv;
using namespace std;

// printRunCMD(funcName, fileName);
// couts command to run as long as path does not have spaces
void printRunCMD(string funcName, string fileName);

// returns file name from user
// fileType prompts user for type input
string getFilePath(string fileType);

int getUserNum(string label);

//void scaleToDisplay(string windowName, CvArr image);

// Checks Valid Mat
// Prints out Help function
bool checkBadInput(Mat& Checkme);

void scaleToDisplay(string WindowName, Mat image);

int getFileIMG(Mat& ImageMat, string fileName, int Flag);

int getFileCAM(Mat& ImageMat);

double pointDist(Point A, Point B);

string tostring(double in);

double sq(double X);

string getTime();

bool waitButton(int pin, int dur);

#endif /* defined(__Initial_Exploration__help__) */
