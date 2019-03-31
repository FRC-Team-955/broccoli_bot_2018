#include <declarative_broccoli_locator.hpp>

DeclarativeBroccoliLocator::DeclarativeBroccoliLocator(cv::Vec3i min_hsv, cv::Vec3i max_hsv, int laplacian_threshold, int area_threshold, int morph_size) : min_hsv(min_hsv), max_hsv(max_hsv), laplacian_threshold(laplacian_threshold), area_threshold(area_threshold) {
    resize_morph_element();
}

DeclarativeBroccoliLocator::DeclarativeBroccoliLocator(cv::FileStorage& file) {
    load_parameters(file);
}

void DeclarativeBroccoliLocator::resize_morph_element() {
    morph_element = getStructuringElement (0, cv::Size (2 * morph_size + 1, 2 * morph_size + 1));
}

cv::Rect DeclarativeBroccoliLocator::locate(cv::Mat& frame, cv::Mat& output_mask) {
    contours.clear();
    cv::cvtColor(frame, color_hsv, cv::COLOR_RGB2HSV);
    cv::inRange(color_hsv, min_hsv, max_hsv, hsv_mask);

    cv::split(frame, color_split);
    cv::Laplacian(color_split[1], laplacian, CV_8UC1);
    cv::inRange(laplacian, cv::Scalar(laplacian_threshold), cv::Scalar(255), laplacian_confirmed);

    cv::bitwise_and(laplacian_confirmed, hsv_mask, lap_conf_mask_combined);

    cv::morphologyEx(lap_conf_mask_combined, morph_blob, cv::MORPH_CLOSE, morph_element);
    cv::morphologyEx(morph_blob, morph_blob, cv::MORPH_OPEN, morph_element);

    morph_blob.copyTo(output_mask);

    cv::Canny(morph_blob, canny_edges, 128, 128);

    cv::findContours(canny_edges, contours, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_TC89_L1);

    for (auto& cont : contours) cont[cont.size() - 1] = cont[0];

    unsigned int largest = 0;
    int largest_index = -1;
    for (unsigned long i = 0; i < contours.size(); i++) {
        unsigned int area = boundingRect(contours[i]).area();
        if (area > largest && area > area_threshold * area_threshold) {
            largest_index = i;
            largest = area;
        }
    }

    if (largest_index == -1) {
        return cv::Rect();
    } else {
        return boundingRect(contours[largest_index]);
    }
}

#define SAVE(NAME) file << #NAME << NAME
void DeclarativeBroccoliLocator::save_parameters(cv::FileStorage& file) {
    SAVE(min_hsv);
    SAVE(max_hsv);
    SAVE(laplacian_threshold);
    SAVE(area_threshold);
    SAVE(morph_size);
}
#undef SAVE

#define LOAD(NAME) file[#NAME] >> NAME
void DeclarativeBroccoliLocator::load_parameters(cv::FileStorage& file) {
    LOAD(min_hsv);
    LOAD(max_hsv);
    LOAD(laplacian_threshold);
    LOAD(area_threshold);
    LOAD(morph_size);
    resize_morph_element();
}
#undef LOAD
