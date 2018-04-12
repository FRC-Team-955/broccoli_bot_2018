#pragma once
#include <librealsense2/rs.hpp>
#include <librealsense2/hpp/rs_internal.hpp>
#include <iostream>
#include <fstream>

//Y'all gotta be compatible with legacy crap
class LegacyLoader {
	public:
		LegacyLoader(char* file_name);
		rs2::frameset wait_for_frames();
	private:
		std::ifstream input_file; 
    	rs2::syncer sync;
		rs2::software_device dev;
		const static int frame_width = 1920;
		const static int frame_height = 1080;
		const static int data_length_color = frame_width * frame_height * 3;
		const static int data_length_depth = frame_width * frame_height * 2;
		const static int length_s = 60;
		const static int framerate = 30;
		const static int frames = length_s * framerate;
		char rgb_buffer[data_length_color];
		char depth_buffer[data_length_color];
};
