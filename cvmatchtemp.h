#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <iostream>
#include <stdio.h>
#include "help.h"

using namespace std;
using namespace cv;


Point cvmatchtemp( Mat img, Mat templ, int match_method) {
    Mat result, img_display;
    int matchReturn = 1;
    double bound;
    string trackbar_label = "Method: \n 0: SQDIFF \n 1: SQDIFF NORMED \n 2: TM CCORR \n 3: TM CCORR NORMED \n 4: TM COEFF \n 5: TM COEFF NORMED";
    cout << "\nMatch Type: " << match_method << endl;
    
//    SETTING THE THRESHOLD FOR MATCH
    switch (match_method) {
        case 0:
            bound = 9000000;
            break;
        case 1:
            bound = .08;
            break;
        case 2:
            bound = 40000000;
            break;
        case 3:
            bound = 0.962;
            break;
        case 4:
            bound = 4e+7;
            break;
        case 5:
            bound = 0.85;
            break;
        default:
            bound = 0;
    }
    
//    COPY IMG FOR DISPLAY:
    img.copyTo(img_display);
    
//    CREATE RESULT MAT && PERFORM MATCHTEMPLATE:
    int result_cols =  img.cols - templ.cols + 1;
    int result_rows = img.rows - templ.rows + 1;
    result.create( result_rows, result_cols, CV_32FC1 );
    matchTemplate( img, templ, result, match_method );
    
//    FIND MAX AND MINS CHOOSE BEST MATCH POINT & EVALUATE POINT:
    double minVal; double maxVal; Point minLoc; Point maxLoc;
    Point matchLoc;
    minMaxLoc( result, &minVal, &maxVal, &minLoc, &maxLoc, Mat() );
    if( match_method  == CV_TM_SQDIFF || match_method == CV_TM_SQDIFF_NORMED ) { // QDIFF and SQDIFF_NORMED, best matches are lower values.
        matchLoc = minLoc;
        cout << "minVal = " << minVal << endl << "maxVal = " << maxVal << endl << "Bound: " << bound << endl;
        if (minVal > bound) {cout << "No Match Found" << endl; matchReturn = 0;
        }
    }
    else { // For all the other methods, the higher the better
        matchLoc = maxLoc;
        cout << "maxVal = " << maxVal << endl << "minVal = " << minVal << endl << "Bound: " << bound << endl;
        if (maxVal < bound) {cout << "No Match Found" << endl; matchReturn = 0;
        }
    }
    
//    OUTPUT:
    if (matchReturn > 0 && true) {
        cout << "Match Found!" << endl;}
        //normalize( result, result, 0, 1, NORM_MINMAX, -1, Mat() );
        rectangle( img_display, matchLoc, Point( matchLoc.x + templ.cols , matchLoc.y + templ.rows ), Scalar::all(0), 2, 8, 0 );
        //rectangle( result, matchLoc, Point( matchLoc.x + templ.cols , matchLoc.y + templ.rows ), Scalar::all(0), 2, 8, 0 );
        //namedWindow("IN", WINDOW_NORMAL);
        //namedWindow("OUT", WINDOW_NORMAL);
        imshow( "IN" , img_display );
        //imshow( "OUT", result );
//        waitKey(0);
    //cout << "Object Found?  " << (char)waitKey(0) << endl;
//    }
    matchLoc.x = matchLoc.x+(templ.cols/2);
    matchLoc.y = matchLoc.y+(templ.rows/2);
    
    return matchLoc;//Point( matchLoc.x + templ.cols , matchLoc.y + templ.rows );
}

