#include <opencv2/core.hpp>
#include <stdint.h>

struct RGBDFrame {
    cv::Mat color;
    cv::Mat depth;
    uint64_t timestamp_ms;
};
