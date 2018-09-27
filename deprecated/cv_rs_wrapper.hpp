#pragma once

#include <librealsense2/rs.hpp>
#include <opencv2/core.hpp>
#include <string.h>

class CVRealsenseWrapper {
	public:
		CVRealsenseWrapper(cv::Size depth_scale, cv::Size color_scale, int framerate);
		inline float get_depth_scaling_factor();
		void wait_for_frames(cv::Mat& depth_frame, cv::Mat& color_frame);
		float depth_scaling_factor;
	private:
		rs2::pipeline pipe;
		rs2::align align;
};
