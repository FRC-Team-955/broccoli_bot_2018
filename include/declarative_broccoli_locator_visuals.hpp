#pragma once
#include <declarative_broccoli_locator.hpp>

class DeclarativeBroccoliLocatorVisuals {
    public:
        static void init_sliders(DeclarativeBroccoliLocator& locator, std::string window_name);
        static void show_internals(DeclarativeBroccoliLocator& locator);
        static void draw_contours(DeclarativeBroccoliLocator& locator, cv::Mat& image, cv::Scalar color);
};
