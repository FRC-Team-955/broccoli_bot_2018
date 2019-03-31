#pragma once

#include <opencv2/core.hpp>

class ObjectLocator {
    public:
        virtual cv::Rect locate(cv::Mat& frame, cv::Mat& outptu_mask) = 0; 
        virtual void load_parameters(cv::FileStorage& file) = 0;
        virtual void save_parameters(cv::FileStorage& file) = 0;
        virtual ~ObjectLocator() {}
};
