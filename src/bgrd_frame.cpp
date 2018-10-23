#include <bgrd_frame.hpp>

RGBDFrame::RGBDFrame(cv::Mat bgr, cv::Mat depth, uint64_t timestamp_ms) 
    : bgr(bgr), depth(depth), timestamp_ms(timestamp_ms) {}
