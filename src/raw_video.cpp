#include <raw_video.hpp>

//TODO: Clean this up!
//Invalid size checking
RawVideo::RawVideo(std::string working_dir, Mode mode, cv::Size& frame_size) : working_dir(working_dir), mode(mode) {
	if (mode == Mode::Read) {
		if (!opendir(working_dir.c_str())) {
			std::cerr << "Directory \'" << working_dir << "\' does not exist." << std::endl;
			exit(EXIT_FAILURE);
		}
		index++;
		std::string bgr_name = working_dir + "/bgr-" + std::to_string(index) + ".png";
		if (access(bgr_name.c_str() , F_OK ) != -1) {
			cv::Mat sample = cv::imread(bgr_name);
			frame_size = cv::Size(sample.rows, sample.cols);
		} else {
			std::cerr << "No data, " << bgr_name << " does not exist." << std::endl;
			exit(EXIT_FAILURE);
		}
	} else {
		if (!opendir(working_dir.c_str())) {
			mkdir(working_dir.c_str(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
		}
	}
}

bool RawVideo::next(cv::Mat& color_frame, cv::Mat& depth_frame) {
	index++;
	std::string dep_name = working_dir + "/dep-" + std::to_string(index) + ".png";
	std::string bgr_name = working_dir + "/bgr-" + std::to_string(index) + ".png";
	if (mode == Mode::Write) {
		cv::imwrite(dep_name, depth_frame);
		cv::imwrite(bgr_name, color_frame);
		return true;
	} else {
		if (access(dep_name.c_str(), F_OK ) != -1 && access(bgr_name.c_str(), F_OK ) != -1) {
			depth_frame = cv::imread(dep_name);
			color_frame = cv::imread(bgr_name);
			return true;
		} else {
			return false;
		}
	}
}

void RawVideo::reset_index() {
	index = 1;
}
