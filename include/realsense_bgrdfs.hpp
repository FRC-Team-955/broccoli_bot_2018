#pragma once

#include <librealsense2/rs.hpp>
#include <opencv2/core.hpp>
#include <bgrd_frame_source.hpp>

class RealSenseBGRDFrameSource : public BGRDFrameSource {
    public:
        RealSenseBGRDFrameSource(cv::Size size, int fps);
        BGRDFrame next();

    private:
		rs2::pipeline pipe;
		rs2::align align;
};
