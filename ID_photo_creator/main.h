#ifndef main_H
#define main_H

#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <opencv/cv.h>
#include <opencv/highgui.h>
#include <opencv2/opencv.hpp>
#include <opencv2/objdetect/objdetect.hpp>
#include <opencv2/ml/ml.hpp>
#include <string>

using namespace cv;
using namespace cv::ml;
using namespace std;

const double PI = 3.1415926;
extern int faceIndex;

//---------------------------------------------------------------------------------- Mouse Event

void actionForFace(int faceIndex);
void onMouse(int event, int x, int y, int flags, void* userdata);

extern vector<Rect> faces;
extern Mat img;
extern Mat face_detected_img;
extern Mat small_rect_img;
extern Mat Grabcut_result;
extern float* magnitude_img;

//---------------------------------------------------------------------------------- Grabcut 관련

bool isBottomRowAllWhite(const Mat& Grabcut_result);
Mat GraphCut_mask(Mat input, Mat input_with_mask);
Mat shiftImageDown(Mat Grabcut_result);
Mat MakeResultDown(Mat Grabcut_result);
Mat ChangeBackgroundColor(Mat Grabcut_result);
Mat ImgResize(Mat Grabcut_result);
Mat ImgRotate(Mat Grabcut_result);
void menu(Mat Grabcut_result);


//---------------------------------------------------------------------------------- Edge 관련

void Edge_Analysis(float* magnitude);
int HairEdge(int faceIndex, float* magnitude);




#endif