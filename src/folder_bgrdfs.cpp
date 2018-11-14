#include <folder_bgrdfs.hpp>

FolderBGRDFrameSource::FolderBGRDFrameSource(char* directory) : 
    bgr_framerdr(directory, (char*)"%s/bgr-%i.png"), 
    depth_framerdr(directory, (char*)"%s/dep-%i.png") {}

BGRDFrame FolderBGRDFrameSource::next() {
    return BGRDFrame(bgr_framerdr.next(), depth_framerdr.next());
}
