#include <string>
#include <chrono>

#include <gflags/gflags.h>

#include "error_code.h"
#include "log.h"
#include "image_quilting.h"

using std::string;
using namespace cv;

DEFINE_string(input, "", "input image path");
DEFINE_int32(block_size, 20, "block size");
DEFINE_double(overlap, 1.0 / 6.0, "overlap ratio of block_size");
DEFINE_double(scale, 2.0, "scaling w.r.t. to image size");
DEFINE_double(tolerance, 0.1, "error tolerance");
DEFINE_string(output, "", "output image path");

int main(int argc, char* argv[]) {
    gflags::SetVersionString("1.0.0");
    gflags::SetUsageMessage("Usage : ./ImageQuilting ");
    gflags::ParseCommandLineFlags(&argc, &argv, true);
    LOGI("input:%s,block_size:%d,overlap:%f,scale:%f,tolerance:%f,output:%s\n",
        FLAGS_input.c_str(), FLAGS_block_size, FLAGS_overlap, FLAGS_scale, FLAGS_tolerance, FLAGS_output.c_str());
    Mat img = imread(FLAGS_input);
    if (img.data == nullptr) {
        LOGE("Fail to open input img! Image path: %s\n", FLAGS_input.c_str());
        return IMG_EMPTY;
    }
    Mat texture;
    ImageQuiltingParams params;
    params.output_size = Size(int(img.size().width * FLAGS_scale), int(img.size().height * FLAGS_scale));
    params.block_size = FLAGS_block_size;
    if (FLAGS_overlap > 0.0 && FLAGS_overlap < 1.0) {
        params.overlap = int(FLAGS_block_size * FLAGS_overlap);
    } else {
        params.overlap = int(FLAGS_block_size / 6.0);
    }
    params.error_tolerance = FLAGS_tolerance;
    ImageQuilting image_quilting(params);
    auto time_start = std::chrono::system_clock::now();
    image_quilting.GenTexture(img, texture);
    auto time_end = std::chrono::system_clock::now();
    auto process_time = std::chrono::duration_cast<std::chrono::milliseconds>(time_end - time_start);
    LOGI("Image quilting generate texture takes time:%lldms,%fs,input size:(%d,%d),output_size(%d,%d)\n",
        process_time.count(), process_time.count() / 1e3, img.rows, img.cols, texture.rows, texture.cols);
    if (FLAGS_output.empty()) {
        imshow("output texture", texture);
        waitKey();
    } else {
        imwrite(FLAGS_output, texture);
    }
    return OK;
}