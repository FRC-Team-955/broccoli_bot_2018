#include <realsense_bgrdfs.hpp>

BGRDFrame RealSenseBGRDFrameSource::next() {
    return BGRDFrame(cv::Mat(), cv::Mat());
}
