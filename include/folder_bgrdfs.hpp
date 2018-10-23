#pragma once

#include <opencv2/core.hpp>
#include <bgrd_frame_source.hpp>
#include <frame_io.hpp>
#include <optional>
#include <dirent.h>
#include <sys/types.h>

class FolderRGBDFrameSource : public RGBDFrameSource {
    public:
        FolderRGBDFrameSource(char* directory);
        std::optional<RGBDFrame> next();

    private:
        FrameReader bgr_framerdr;
        FrameReader depth_framerdr;
};
