#include <stdio.h>
#include <iostream>

#include <opencv2/opencv.hpp>
#include <gtest/gtest.h>

#include "image_quilting.h"
#include "print.h"

using std::cout;
using namespace cv;

template<typename T>
bool IsMatEqual(const Mat& a, const Mat& b, T type);

class ImageQuiltingTest : public ::testing::Test
{
protected:
    ImageQuilting image_quilting;
    // SetUp & TearDown will be called for every test case
    virtual void SetUp() {
        ImageQuiltingParams params;
        params.output_size = Size(4, 4);
        params.block_size = 4;
        params.overlap = 2;
        params.error_tolerance = 0.1;
        image_quilting.SetParams(params);
    }
    virtual void TearDown() {

    }
};

TEST_F(ImageQuiltingTest, TestInit) {
    ImageQuiltingParams params;
    params.output_size = Size(6, 7);
    params.block_size = 4;
    params.overlap = 2;
    params.error_tolerance = 0.1;
    ImageQuilting test_image_quilting(params);
    EXPECT_EQ(Size(6, 6), test_image_quilting.GetOutputSize());
}

TEST_F(ImageQuiltingTest, TestSetOutputSize) {
    image_quilting.SetOutputSize(Size(6, 7));
    EXPECT_EQ(Size(6, 6), image_quilting.GetOutputSize());
    image_quilting.SetOutputSize(Size(9, 8));
    EXPECT_EQ(Size(8, 8), image_quilting.GetOutputSize());
    image_quilting.SetOutputSize(Size(9, 7));
    EXPECT_EQ(Size(8, 6), image_quilting.GetOutputSize());
    image_quilting.SetOverlap(1);
    image_quilting.SetOutputSize(Size(6, 7));
    EXPECT_EQ(Size(4, 7), image_quilting.GetOutputSize());
}

TEST_F(ImageQuiltingTest, TestGetPatch) {
    int block_size = 2;
    image_quilting.SetBlockSize(2);
    Mat source = (Mat_<int>(3, 3) << 0, 1, 2, 3, 4, 5, 6, 7, 8);
    int rows = source.rows;
    int cols = source.cols;
    Mat patch, patch_truth;
    for (int i = 0; i <= rows - block_size; i++) {
        for (int j = 0; j <= cols - block_size; j++) {
            patch_truth = source(Range(i, i + block_size), Range(j, j + block_size));
            image_quilting.GetPatch(source, i, j, patch);
            EXPECT_EQ(true, IsMatEqual(patch_truth, patch, int(1)));
        }
    }
}

TEST_F(ImageQuiltingTest, TestGetAllOverlapRegion) {
    image_quilting.SetBlockSize(2);
    image_quilting.SetOverlap(1);
    Mat source = (Mat_<int>(3, 4) << 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11);
    Mat overlaps;
    // horizontal
    image_quilting.GetAllOverlapRegionHorizontal(source, overlaps);
    PrintMat(source, "source");
    PrintMat(overlaps, "overlaps");
    image_quilting.GetAllOverlapRegionHorizontal(source.reshape(2, 3), overlaps);
    PrintMat(overlaps, "overlaps");
    // vertical
    image_quilting.GetAllOverlapRegionVertical(source, overlaps);
    PrintMat(source, "source");
    PrintMat(overlaps, "overlaps");
    image_quilting.GetAllOverlapRegionVertical(source.reshape(2, 3), overlaps);
    PrintMat(overlaps, "overlaps");
    // both
    image_quilting.GetAllOverlapRegionBoth(source, overlaps);
    PrintMat(source, "source");
    PrintMat(overlaps, "overlaps");
    image_quilting.GetAllOverlapRegionBoth(source.reshape(2, 3), overlaps);
    PrintMat(overlaps, "overlaps");
    Mat overlap_region;
    image_quilting.GetOverlap(source, 0, 0, overlap_region, HORIZONTAL);
    PrintMat(overlap_region, "horizontal overlap");
    image_quilting.GetOverlap(source, 0, 0, overlap_region, VERTICAL);
    PrintMat(overlap_region, "vertical overlap");
    image_quilting.GetOverlap(source, 0, 0, overlap_region, BOTH);
    PrintMat(overlap_region, "both overlap");
}

TEST_F(ImageQuiltingTest, TestFindPatch) {
    // FindPatchHorizontal
    Mat ref_region = (Mat_<unsigned char>(4, 2) << 0, 1, 2, 3, 4, 5, 6, 7);
    Mat cur_block = (Mat_<unsigned char>(4, 4) << 0, 1, 2, 3, 3, 5, 6, 8, 8, 10, 10, 11, 12, 13, 14, 15);
    PrintMat(ref_region, "ref region");
    PrintMat(cur_block, "cur_block");
    image_quilting.FindPatchHorizontal(ref_region, cur_block);
    PrintMat(cur_block, "cur_block");
    cur_block = (Mat_<unsigned char>(4, 4) << 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15);
    transpose(ref_region, ref_region);
    image_quilting.FindPatchVertical(ref_region, cur_block);
    PrintMat(cur_block, "cur_block");
    cur_block = (Mat_<unsigned char>(4, 4) << 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15);
    Mat top_region = ref_region, left_region;
    transpose(ref_region, left_region);
    image_quilting.FindPatchBoth(left_region, top_region, cur_block);
    PrintMat(cur_block, "cur_block");
}

TEST_F(ImageQuiltingTest, TestGenTexture) {
    ImageQuiltingParams params;
    params.output_size = Size(128, 128);
    params.block_size = 20;
    params.overlap = 4;
    params.error_tolerance = 0.1;
    ImageQuilting test_image_quilting(params);
    Mat exemplar = imread("../imgs/t1.png");
    Mat texture;
    ASSERT_FALSE(exemplar.data == nullptr);
    test_image_quilting.GenTexture(exemplar, texture);
    imshow("exemplar", exemplar);
    waitKey();
    imshow("texture with first row", texture);
    waitKey();
}

template<typename T>
bool IsMatEqual(const Mat& a, const Mat& b, T type) {
    if (a.size() != b.size() || a.channels() != b.channels() || a.type() != b.type()) {
        return false;
    }
    int rows = a.rows;
    int cols = a.cols;
    int channels = a.channels();
    for (int i = 0; i < rows; i++) {
        const T* p_a = a.ptr<T>(i);
        const T* p_b = b.ptr<T>(i);
        for (int j = 0; j < cols; j++) {
            for (int c = 0; c < channels; c++) {
                EXPECT_FLOAT_EQ(p_a[c], p_b[c]);
            }
            p_a += channels;
            p_b += channels;
        }
    }
    return true;
}