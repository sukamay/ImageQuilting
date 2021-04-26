#include "image_quilting.h"

#include <random>

#include "print.h"

ImageQuilting::ImageQuilting()
    :output_size_(Size(148, 148)), block_size_(20), overlap_(4), error_tolerance_(0.1) {

}

ImageQuilting::ImageQuilting(ImageQuiltingParams& params)
    : block_size_(params.block_size),
    overlap_(params.overlap), error_tolerance_(params.error_tolerance) {
    output_size_.width = block_size_ + (block_size_ - overlap_) *
        ((params.output_size.width - block_size_) / (block_size_ - overlap_));
    output_size_.height = block_size_ + (block_size_ - overlap_) *
        ((params.output_size.height - block_size_) / (block_size_ - overlap_));
}

ImageQuilting::~ImageQuilting() {
}

ERROR_CODE ImageQuilting::GenTexture(const Mat& exemplar, Mat& texture) {
    if (exemplar.data == nullptr) {
        return IMG_EMPTY;
    }
    if (exemplar.type() > CV_8UC4) {
        return IMG_INVALID;
    }
    if (output_size_.width < block_size_ || output_size_.height < block_size_) {
        return PARAM_INVALID;
    }
    texture = Mat(output_size_, exemplar.type(), Scalar(0, 0, 0));
    int rows = texture.rows;
    int cols = texture.cols;
    int channels = texture.channels();
    ERROR_CODE  ret;
    // step 1: preprocess
    Mat horizontal_overlaps, vertical_overlaps, both_overlaps;
    GetAllOverlapRegionHorizontal(exemplar, horizontal_overlaps);
    GetAllOverlapRegionVertical(exemplar, vertical_overlaps);
    GetAllOverlapRegionBoth(exemplar, both_overlaps);
    flann::KDTreeIndexParams kdtree_index_params(4);
    flann::Index horizontal_kdtree(horizontal_overlaps, kdtree_index_params);
    flann::Index vertical_kdtree(vertical_overlaps, kdtree_index_params);
    flann::Index both_kdtree(both_overlaps, kdtree_index_params);
    flann::SearchParams ann_search_params(32, error_tolerance_);
    // step 2-1: fill first block
    Mat patch;
    ret = GetRandomPatch(exemplar, patch);
    if (ret != OK) { return ret; }
    patch.copyTo(texture(Range(0, block_size_), Range(0, block_size_)));
    // step 2-2: fill first row
    int scan_step = block_size_ - overlap_;
    int row_end = rows - block_size_;
    int col_end = cols - block_size_;
    int overlap_cols = exemplar.cols - block_size_ + 1;
    int ei, ej;
    Mat queries, indices, dists, overlap_region;
    int* p_indices;
    for (int j = scan_step; j <= col_end; j += scan_step) {
        GetOverlap(texture, 0, j, queries, HORIZONTAL);
        overlap_region = queries.reshape(channels, block_size_);
        queries.convertTo(queries, CV_32F);
        horizontal_kdtree.knnSearch(queries, indices, dists, 1, ann_search_params);
        p_indices = indices.ptr<int>(0);
        GetUVFromIndex(overlap_cols, p_indices[0], ei, ej);
        GetPatch(exemplar, ei, ej, patch);
        FindPatchHorizontal(overlap_region, patch);
        patch.copyTo(texture(Range(0, block_size_), Range(j, j + block_size_)));
    }
    // step 2-3: fill first col
    for (int i = scan_step; i <= row_end; i += scan_step) {
        GetOverlap(texture, i, 0, queries, VERTICAL);
        overlap_region = queries.reshape(channels, overlap_);
        queries.convertTo(queries, CV_32F);
        vertical_kdtree.knnSearch(queries, indices, dists, 1, ann_search_params);
        p_indices = indices.ptr<int>(0);
        GetUVFromIndex(overlap_cols, p_indices[0], ei, ej);
        GetPatch(exemplar, ei, ej, patch);
        FindPatchVertical(overlap_region, patch);
        patch.copyTo(texture(Range(i, i + block_size_), Range(0, block_size_)));
    }
    // step 2-4: fill all
    Mat left_region, top_region;
    for (int i = scan_step; i <= row_end; i += scan_step) {
        for (int j = scan_step; j <= col_end; j += scan_step) {
            GetOverlap(texture, i, j, queries, BOTH);
            GetOverlap(texture, i, j, left_region, HORIZONTAL);
            GetOverlap(texture, i, j, top_region, VERTICAL);
            left_region = left_region.reshape(channels, block_size_);
            top_region = top_region.reshape(channels, overlap_);
            queries.convertTo(queries, CV_32F);
            both_kdtree.knnSearch(queries, indices, dists, 1, ann_search_params);
            p_indices = indices.ptr<int>(0);
            GetUVFromIndex(overlap_cols, p_indices[0], ei, ej);
            GetPatch(exemplar, ei, ej, patch);
            FindPatchBoth(left_region, top_region, patch);
            patch.copyTo(texture(Range(i, i + block_size_), Range(j, j + block_size_)));
        }
    }
    return OK;
}

ERROR_CODE ImageQuilting::GetPatch(const Mat& source, int i, int j, Mat& patch) {
    if (source.data == nullptr) {
        return IMG_EMPTY;
    }
    if (i < 0 || i >(source.rows - block_size_) || j < 0 || j >(source.cols - block_size_)) {
        return PARAM_INVALID;
    }
    source(Range(i, i + block_size_), Range(j, j + block_size_)).copyTo(patch);
    return OK;
}

ERROR_CODE ImageQuilting::GetRandomPatch(const Mat& source, Mat& patch) {
    std::random_device rd;
    std::default_random_engine e(rd());
    std::uniform_int_distribution<int> row_dis(0, source.rows - block_size_);
    std::uniform_int_distribution<int> col_dis(0, source.cols - block_size_);
    return GetPatch(source, row_dis(e), col_dis(e), patch);
}

ERROR_CODE ImageQuilting::GetAllOverlapRegionHorizontal(const Mat& source, Mat& overlaps) {
    int rows = source.rows - block_size_;
    int cols = source.cols - block_size_;
    int overlap_dim = block_size_ * overlap_ * source.channels();
    int overlap_num = (rows + 1) * (cols + 1);
    overlaps = Mat(Size(overlap_dim, overlap_num), CV_8U);
    Mat cur_overlap_region;
    int cur_index = 0;
    for (int i = 0; i <= rows; i++) {
        for (int j = 0; j <= cols; j++) {
            source(Range(i, i + block_size_), Range(j, j + overlap_)).copyTo(cur_overlap_region);
            cur_overlap_region.reshape(1, 1).copyTo(overlaps.row(cur_index));
            cur_index++;
        }
    }
    overlaps.convertTo(overlaps, CV_32F);
    return OK;
}

ERROR_CODE ImageQuilting::GetAllOverlapRegionVertical(const Mat& source, Mat& overlaps) {
    int rows = source.rows - block_size_;
    int cols = source.cols - block_size_;
    int overlap_dim = block_size_ * overlap_ * source.channels();
    int overlap_num = (rows + 1) * (cols + 1);
    overlaps = Mat(Size(overlap_dim, overlap_num), CV_8U);
    Mat cur_overlap_region;
    int cur_index = 0;
    for (int i = 0; i <= rows; i++) {
        for (int j = 0; j <= cols; j++) {
            source(Range(i, i + overlap_), Range(j, j + block_size_)).copyTo(cur_overlap_region);
            cur_overlap_region.reshape(1, 1).copyTo(overlaps.row(cur_index));
            cur_index++;
        }
    }
    overlaps.convertTo(overlaps, CV_32F);
    return OK;
}

ERROR_CODE ImageQuilting::GetAllOverlapRegionBoth(const Mat& source, Mat& overlaps) {
    int rows = source.rows - block_size_;
    int cols = source.cols - block_size_;
    int overlap_one_side_dim = block_size_ * overlap_ * source.channels();
    int overlap_dim = overlap_one_side_dim << 1;
    int overlap_num = (rows + 1) * (cols + 1);
    overlaps = Mat(Size(overlap_dim, overlap_num), CV_8U);
    Mat cur_overlap_region;
    int cur_index = 0;
    for (int i = 0; i <= rows; i++) {
        for (int j = 0; j <= cols; j++) {
            source(Range(i, i + overlap_), Range(j, j + block_size_)).copyTo(cur_overlap_region);
            cur_overlap_region.reshape(1, 1).copyTo(overlaps(Range(cur_index, cur_index + 1), Range(0, overlap_one_side_dim)));
            source(Range(i, i + block_size_), Range(j, j + overlap_)).copyTo(cur_overlap_region);
            cur_overlap_region.reshape(1, 1).copyTo(overlaps(Range(cur_index, cur_index + 1), Range(overlap_one_side_dim, overlap_dim)));
            cur_index++;
        }
    }
    overlaps.convertTo(overlaps, CV_32F);
    return OK;
}

ERROR_CODE ImageQuilting::GetOverlap(const Mat& source, int i, int j, Mat& overlap_region, DIRECTION direction) {
    int overlap_dim = block_size_ * overlap_ * source.channels();
    if (direction == BOTH) {
        overlap_region = Mat(Size(overlap_dim << 1, 1), CV_8U);
    } else {
        overlap_region = Mat(Size(overlap_dim, 1), CV_8U);
    }
    Mat temp_region;
    switch (direction)     {
    case HORIZONTAL:
        source(Range(i, i + block_size_), Range(j, j + overlap_)).copyTo(temp_region);
        temp_region.reshape(1, 1).copyTo(overlap_region);
        break;
    case VERTICAL:
        source(Range(i, i + overlap_), Range(j, j + block_size_)).copyTo(temp_region);
        temp_region.reshape(1, 1).copyTo(overlap_region);
        break;
    case BOTH:
        source(Range(i, i + overlap_), Range(j, j + block_size_)).copyTo(temp_region);
        temp_region.reshape(1, 1).copyTo(overlap_region(Range(0, 1), Range(0, overlap_dim)));
        source(Range(i, i + block_size_), Range(j, j + overlap_)).copyTo(temp_region);
        temp_region.reshape(1, 1).copyTo(overlap_region(Range(0, 1), Range(overlap_dim, overlap_dim << 1)));
        break;
    default:
        break;
    }
    return OK;
}

ERROR_CODE ImageQuilting::FindPatchHorizontal(const Mat& ref_region, Mat& cur_block) {
    int channels = ref_region.channels();
    Mat dp(Size(overlap_ + 2, block_size_), CV_32S, Scalar(INT_MAX));
    Mat dp_index(Size(overlap_ + 2, block_size_), CV_8U, Scalar(3));
    // step 1: initial first row
    int* p_dp = dp.ptr<int>(0);
    int* p_last_dp;
    unsigned char* p_dp_index = dp_index.ptr<unsigned char>(0);
    const unsigned char* p_ref_region = ref_region.ptr<unsigned char>(0);
    unsigned char* p_cur_block = cur_block.ptr<unsigned char>(0);
    int norm, cur_error, cur_index;
    for (int j = 0; j < overlap_; j++) {
        norm = 0;
        for (int c = 0; c < channels; c++) {
            norm += (int(p_ref_region[c]) - int(p_cur_block[c])) * (int(p_ref_region[c]) - int(p_cur_block[c]));
        }
        p_ref_region += channels;
        p_cur_block += channels;
        p_dp[j + 1] = norm;
    }
    // step 2: dynamic programming
    for (int i = 1; i < block_size_; i++) {
        p_ref_region = ref_region.ptr<unsigned char>(i);
        p_cur_block = cur_block.ptr<unsigned char>(i);
        p_dp = dp.ptr<int>(i);
        p_last_dp = dp.ptr<int>(i - 1);
        p_dp_index = dp_index.ptr<unsigned char>(i);
        for (int j = 1; j <= overlap_; j++) {
            norm = 0;
            for (int c = 0; c < channels; c++) {
                norm += (int(p_ref_region[c]) - int(p_cur_block[c])) * (int(p_ref_region[c]) - int(p_cur_block[c]));
            }
            p_ref_region += channels;
            p_cur_block += channels;
            cur_error = INT_MAX;
            cur_index = 3;
            for (int k = -1; k <= 1; k++) {
                if (p_last_dp[j + k] < cur_error) {
                    cur_error = p_last_dp[j + k];
                    cur_index = k + 1;
                }
            }
            p_dp[j] = norm + cur_error;
            p_dp_index[j] = cur_index;
        }
    }
    // PrintMat(ref_region, "ref_region");
    // PrintMat(cur_block, "cur_block");
    // PrintMat(dp, "dp");
    // PrintMat(dp_index, "dp_index");
    // step 3: find min error
    p_dp = dp.ptr<int>(block_size_ - 1);
    cur_error = INT_MAX;
    cur_index = -1;
    for (int j = 1; j <= overlap_; j++) {
        if (p_dp[j] < cur_error) {
            cur_error = p_dp[j];
            cur_index = j;
        }
    }
    // step 4: find path
    for (int i = block_size_ - 1; i >= 0; i--) {
        p_dp = dp.ptr<int>(i);
        p_dp_index = dp_index.ptr<unsigned char>(i);
        ref_region(Range(i, i + 1), Range(0, cur_index)).copyTo(cur_block(Range(i, i + 1), Range(0, cur_index)));
        cur_index += (int(p_dp_index[cur_index]) - 1);
    }
    return OK;
}

ERROR_CODE ImageQuilting::FindPatchVertical(const Mat& ref_region, Mat& cur_block) {
    Mat temp_block, temp_region;
    transpose(ref_region, temp_region);
    transpose(cur_block, temp_block);
    FindPatchHorizontal(temp_region, temp_block);
    transpose(temp_block, cur_block);
    return OK;
}

ERROR_CODE ImageQuilting::FindPatchBoth(const Mat& left_region, const Mat& top_region, Mat& cur_block) {
    FindPatchHorizontal(left_region, cur_block);
    FindPatchVertical(top_region, cur_block);
    return OK;
}

ERROR_CODE ImageQuilting::CalMinCostPathHorizontal() {
    return OK;
}
ERROR_CODE ImageQuilting::CalMinCostPathVertical() {
    return OK;
}
ERROR_CODE ImageQuilting::CalMinCostPathBoth() {
    return OK;
}

void ImageQuilting::GetUVFromIndex(const int& cols, int index, int& u, int& v) {
    u = index / cols;
    v = index - u * cols;
}

void ImageQuilting::GetIndexFromUV(const int& cols, int u, int v, int& index) {
    index = u * cols + v;
}

void ImageQuilting::SetParams(ImageQuiltingParams& params) {
    output_size_.width = block_size_ + (block_size_ - overlap_) *
        ((params.output_size.width - block_size_) / (block_size_ - overlap_));
    output_size_.height = block_size_ + (block_size_ - overlap_) *
        ((params.output_size.height - block_size_) / (block_size_ - overlap_));
    block_size_ = params.block_size;
    error_tolerance_ = params.error_tolerance;
    overlap_ = params.overlap;
}

void ImageQuilting::SetOutputSize(Size os) {
    output_size_.width = block_size_ + (block_size_ - overlap_) *
        ((os.width - block_size_) / (block_size_ - overlap_));
    output_size_.height = block_size_ + (block_size_ - overlap_) *
        ((os.height - block_size_) / (block_size_ - overlap_));
}

ImageQuiltingParams ImageQuilting::GetParams() {
    ImageQuiltingParams params;
    params.output_size = output_size_;
    params.block_size = block_size_;
    params.error_tolerance = error_tolerance_;
    params.overlap = overlap_;
    return params;
}