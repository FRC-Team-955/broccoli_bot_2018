#pragma once

#include <librealsense2/rs.hpp>
#include <opencv2/core.hpp>
#include <bgrd_frame_source.hpp>
#include <optional>

class RealSenseRGBDFrameSource : public RGBDFrameSource {
    public:
        RealSenseRGBDFrameSource(cv::Size size, int fps);
        std::optional<RGBDFrame> next();

    private:
		rs2::pipeline pipe;
		rs2::align align;
};
