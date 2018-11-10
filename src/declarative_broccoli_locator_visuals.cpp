#include <declarative_broccoli_locator_visuals.hpp>

void DeclarativeBroccoliLocatorVisuals::init_sliders(DeclarativeBroccoliLocator& locator, std::string window_name) {
    cv::namedWindow(window_name);
    cv::createTrackbar("H min", window_name, &locator.min_hsv[0], 255);
    cv::createTrackbar("H max", window_name, &locator.max_hsv[0], 255);

    cv::createTrackbar("S min", window_name, &locator.min_hsv[1], 255);
    cv::createTrackbar("S max", window_name, &locator.max_hsv[1], 255);

    cv::createTrackbar("V min", window_name, &locator.min_hsv[2], 255);
    cv::createTrackbar("V max", window_name, &locator.max_hsv[2], 255);

    cv::createTrackbar("Lap thresh", window_name, &locator.laplacian_threshold, 1000);
    cv::createTrackbar("Area thresh", window_name, &locator.area_threshold, 2000);
}

#define SHOW(NAME) cv::imshow(#NAME, NAME)
void DeclarativeBroccoliLocatorVisuals::show_internals(DeclarativeBroccoliLocator& locator) {
    if (!locator.hsv_mask.empty()) SHOW(locator.hsv_mask);
    if (!locator.large_blob_mask.empty()) SHOW(locator.large_blob_mask);
    if (!locator.laplacian_confirmed.empty()) SHOW(locator.laplacian_confirmed);
}
#undef SHOW
