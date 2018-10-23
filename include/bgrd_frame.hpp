#pragma once

#include <opencv2/core.hpp>
#include <stdint.h>

struct RGBDFrame {
    cv::Mat bgr;
    cv::Mat depth;
    uint64_t timestamp_ms;
    RGBDFrame(cv::Mat bgr, cv::Mat depth, uint64_t timestamp_ms);
};
