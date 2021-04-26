#include "print.h"

void PrintMat(const Mat& mat, string mat_name) {
    cout << mat_name << ": size:" << mat.size() << ",type:" << mat.type() << ",channels:" << mat.channels() << ",data:\n";
    cout << mat << "\n";
}

void PrintMatHeader(const Mat& mat, string mat_name) {
    cout << mat_name << ": size:" << mat.size() << ",type:" << mat.type() << ",channels:" << mat.channels() << ",data:\n";
}