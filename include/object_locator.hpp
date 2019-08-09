#pragma once

#include <opencv2/core.hpp>
#include <vector>

class ObjectLocator {
    public:
        virtual std::vector<cv::Rect> locate(cv::Mat& frame, cv::Mat& output_mask) = 0; 
        virtual void load_parameters(cv::FileStorage& file) = 0;
        virtual void save_parameters(cv::FileStorage& file) = 0;
        virtual ~ObjectLocator() {}
};
