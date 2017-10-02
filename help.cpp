//
//  help.cpp
//  Initial Exploration
//
//  Created by Russell Ohnemus on 6/18/15.
//  Copyright (c) 2015 Russell Ohnemus. All rights reserved.
//

#include <stdio.h>
#include <unistd.h>
#include <iostream>
#include <sstream>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include "help.h"
#include "wiringPi.h"

void printRunCMD(string funcName, string fileName) {
    char cwd[1024];
    cout << "cd " << getcwd(cwd, sizeof(cwd));// << "/build/Debug";
    cout << " && " << "./" << funcName << " " << fileName << endl << endl;
}

string getFilePath(string fileType) {
    string path;
    cout << "Enter " << fileType << " File Name: ";
    getline(cin,path);
    cout << endl << fileType << " File name: " << path << endl << endl;
    return path;
}

int getUserNum(string label) {
    int num;
    cout << "Enter " << label << ": ";
    cin >> num;
    return num;
}

bool checkBadInput(Mat& Checkme) {
    if (! Checkme.data) {
        cout << "Image not found or Invalid Image type\n" << endl;
        return true;
    }
    return false;
}

Mat makeResult(Mat reff, Mat check) {
    int cols = check.cols - reff.cols + 1;
    int rows = check.rows - reff.rows + 1;
    Mat out;
    out.zeros(rows, cols, CV_32FC2);
    return out;
}

void scaleToDisplay(string WindowName, Mat image){
    int display_X = 1280;
    int display_Y = 852;
    Mat dest = Mat::zeros(display_X, display_Y,CV_64F);
    double scale = MIN(display_X/image.cols, display_Y/image.rows);
    resize(image, dest, Size(0, 0), scale, scale);
    resizeWindow(WindowName, display_X, display_Y);
    moveWindow(WindowName, 0, 0);
    imshow(WindowName, dest);
    return;
}

int getFileIMG(Mat& ImageMat, string fileName, int Flag){
    ImageMat = imread(fileName, CV_LOAD_IMAGE_COLOR);
    if (checkBadInput(ImageMat)) return 0;
    else return 1;
}

int getFileCAM(Mat& ImageMat){
    VideoCapture cap(0);
    if (!cap.isOpened()) {
        cerr << "Could not open Camera. Quiting..." << endl;
        return 0;
    }
    cap >> ImageMat;
//    double y = getFileIMG(ImageMat, "L.jpg", CV_LOAD_IMAGE_COLOR);
//    if (!y) return 0;
    return 1;
}

double pointDist(Point A, Point B){
    double out = sqrt( sq((A.x-B.x)) + sq((A.y-B.y)));
    return out;
}

double sq(double X){
    return pow(X, 2);
}

string getTime(){
    time_t t = time(0);
    struct tm* now = localtime( &t );
    char buffer[80];
    strftime(buffer, 80, "%Y-%m-%d-%H:%M:%S", now);
    return buffer;
}

string tostring(double in){
    ostringstream str;
    str << in;
    return str.str();
}

bool waitButton(int pin, int dur){
    //if (dur > 0) for(int c = 0; c < dur; c++) if (digitalRead(pin)) return true;
    for(;;) if (digitalRead(pin) == 1) return true;
    return false;
}
