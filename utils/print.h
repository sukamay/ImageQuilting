#ifndef PRINT_H
#define PRINT_H

#include <iostream>
#include <string>

#include <opencv2/opencv.hpp>

using std::cout;
using std::string;
using namespace cv;

void PrintMat(const Mat& mat, string mat_name = "mat");
void PrintMatHeader(const Mat& mat, string mat_name = "mat");

#endif