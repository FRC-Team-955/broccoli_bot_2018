#pragma once

#include <opencv2/core.hpp>
#include <bgrd_frame_source.hpp>
#include <frame_io.hpp>
#include <dirent.h>
#include <sys/types.h>

class FolderBGRDFrameSource : public BGRDFrameSource {
    public:
        FolderBGRDFrameSource(char* directory);
        BGRDFrame next();

    private:
        FrameReader bgr_framerdr;
        FrameReader depth_framerdr;
};
