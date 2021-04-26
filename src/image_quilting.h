#ifndef IMAGE_QUILTING_H
#define IMAGE_QUILTING_H

#include <opencv2/opencv.hpp>
#include <opencv2/core.hpp>
#include <opencv2/flann/miniflann.hpp>
#include <opencv2/flann/flann_base.hpp>
#include <opencv2/ml.hpp>

#include "error_code.h"
#include "log.h"

using namespace cv;

struct ImageQuiltingParams
{
    Size output_size;
    int block_size;
    double error_tolerance;
    int overlap;
};

enum DIRECTION
{
    HORIZONTAL,
    VERTICAL,
    BOTH
};

class ImageQuilting
{
private:
    Size output_size_;
    int block_size_;
    double error_tolerance_;
    int overlap_;

public:
    ImageQuilting(ImageQuiltingParams& params);
    ImageQuilting();
    ~ImageQuilting();

    ERROR_CODE GenTexture(const Mat& exemplar, Mat& texture);
    // tool function
    ERROR_CODE GetPatch(const Mat& source, int i, int j, Mat& patch);
    ERROR_CODE GetRandomPatch(const Mat& source, Mat& patch);

    ERROR_CODE GetAllOverlapRegionHorizontal(const Mat& source, Mat& overlaps);
    ERROR_CODE GetAllOverlapRegionVertical(const Mat& source, Mat& overlaps);
    ERROR_CODE GetAllOverlapRegionBoth(const Mat& source, Mat& overlaps);
    ERROR_CODE GetOverlap(const Mat& source, int i, int j, Mat& overlap_region, DIRECTION direction);

    ERROR_CODE FindPatchHorizontal(const Mat& ref_region, Mat& cur_block);
    ERROR_CODE FindPatchVertical(const Mat& ref_region, Mat& cur_block);
    ERROR_CODE FindPatchBoth(const Mat& left_region, const Mat& top_region, Mat& cur_block);

    ERROR_CODE CalMinCostPathHorizontal();
    ERROR_CODE CalMinCostPathVertical();
    ERROR_CODE CalMinCostPathBoth();

    void GetUVFromIndex(const int& cols, int index, int& u, int& v);
    void GetIndexFromUV(const int& cols, int u, int v, int& index);
    // set/get data
    void SetParams(ImageQuiltingParams& params);
    void SetOutputSize(Size os);
    inline void SetBlockSize(int bs) { block_size_ = bs; }
    inline void SetErrorTolerance(double et) { error_tolerance_ = et; }
    inline void SetOverlap(int o) { overlap_ = o; }

    ImageQuiltingParams GetParams();
    inline Size GetOutputSize() { return output_size_; }
    inline int GetBlockSize() { return block_size_; }
    inline double GetErrorTolerance() { return error_tolerance_; }
    inline int GetOverlap() { return overlap_; }
};

#endif