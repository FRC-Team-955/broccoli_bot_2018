#include <folder_bgrdfs.hpp>

FolderBGRDFrameSource::FolderBGRDFrameSource(char* directory) : 
    bgr_framerdr(directory, (char*)"%s/bgr-%i.png"), 
    depth_framerdr(directory, (char*)"%s/dep-%i.png") {}

BGRDFrame FolderBGRDFrameSource::next() {
    cv::Mat bgr_frame = bgr_framerdr.next();
    cv::Mat depth_frame = depth_framerdr.next();
    return BGRDFrame(bgr_frame, depth_frame);
}
