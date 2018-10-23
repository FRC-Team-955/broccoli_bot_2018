#include <folder_bgrdfs.hpp>

FolderRGBDFrameSource::FolderRGBDFrameSource(char* directory) : 
    bgr_framerdr(directory, (char*)"%s/bgr-%i.png"), 
    depth_framerdr(directory, (char*)"%s/dep-%i.png") {}

std::optional<RGBDFrame> FolderRGBDFrameSource::next() {
    cv::Mat bgr_frame = bgr_framerdr.next();
    cv::Mat depth_frame = depth_framerdr.next();
    if (bgr_frame.cols > 0 && depth_frame.cols > 0) {
        return std::make_optional(RGBDFrame(bgr_frame, depth_frame, 0));
    } else {
        return {};
    }
}
