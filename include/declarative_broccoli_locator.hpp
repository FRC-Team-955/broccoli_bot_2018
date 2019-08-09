#pragma once

#include <opencv2/opencv.hpp>
#include <bgrd_frame_source.hpp>
#include <object_locator.hpp>
#include <vector>

class DeclarativeBroccoliLocator : public ObjectLocator {
    friend class DeclarativeBroccoliLocatorVisuals;
    public:
        DeclarativeBroccoliLocator(cv::FileStorage& file);
        DeclarativeBroccoliLocator(cv::Vec3i min_hsv, cv::Vec3i max_hsv, int laplacian_threshold, int area_threshold, int morph_size);
        std::vector<cv::Rect> locate(cv::Mat& frame, cv::Mat& output_mask); 

        void load_parameters(cv::FileStorage& file);
        void save_parameters(cv::FileStorage& file);
    private:
        cv::Mat morph_element;
		cv::Mat color_hsv, hsv_mask;
		cv::Mat color_split[3];
		cv::Mat laplacian, laplacian_confirmed;
		cv::Mat lap_conf_mask_combined;
		cv::Mat morph_blob;
		cv::Mat canny_edges;
		cv::Mat large_blob_mask;

        cv::Vec3i min_hsv, max_hsv;
        int laplacian_threshold;
        int area_threshold;
        int morph_size = 5;
		std::vector<std::vector<cv::Point>> contours;
        void resize_morph_element();
};
