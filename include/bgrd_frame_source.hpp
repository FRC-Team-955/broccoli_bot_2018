#pragma once

#include <opencv2/core.hpp>
#include <optional>

struct BGRDFrame {
    cv::Mat bgr;
    cv::Mat depth;
    BGRDFrame(cv::Mat bgr, cv::Mat depth) : bgr(bgr), depth(depth) {}
};

class BGRDFrameSource {
    public: 
        virtual BGRDFrame next() = 0;
};
