#pragma once
#include <iostream>
#include <fstream>
#include <opencv2/core.hpp>

//Y'all gotta be compatible with legacy crap
class LegacyLoader {
	public:
		LegacyLoader(char* file_name);
		void next_frame();
		cv::Mat color_frame;
		cv::Mat depth_frame;
	private:
		std::ifstream input_file; 
		const static int frame_width = 1920;
		const static int frame_height = 1080;
		const static int data_length_color = frame_width * frame_height * 3;
		const static int data_length_depth = frame_width * frame_height * 2;
		const static int length_s = 60;
		const static int framerate = 30;
		const static int frames = length_s * framerate;
};
