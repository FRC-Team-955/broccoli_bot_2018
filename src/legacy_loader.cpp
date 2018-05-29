#include <legacy_loader.hpp>

LegacyLoader::LegacyLoader(char *file_name) {
	input_file = std::ifstream(file_name, std::ios::binary);
	color_frame = cv::Mat::zeros(frame_height, frame_width, CV_8UC3);
	depth_frame = cv::Mat::zeros(frame_height, frame_width, CV_16UC1);
}

bool LegacyLoader::next_frame() {
	return input_file.read((char*)color_frame.data, data_length_color) && 
	input_file.read((char*)depth_frame.data, data_length_depth);
}
