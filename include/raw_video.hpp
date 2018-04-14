#pragma once

#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <sys/stat.h>
#include <unistd.h>
#include <bzlib.h>
#include <iostream>
#include <string>
#include <dirent.h>
#include <sys/stat.h>
#include <stdlib.h>

class RawVideo {
	public:
		enum Mode {
			Read,
			Write,
		};
		RawVideo(std::string working_dir, Mode mode, cv::Size& frame_size);
		bool next(cv::Mat& color_frame, cv::Mat& depth_frame); //Either read or write frames depending on mode
		void reset_index();
	private:
		Mode mode;
		FILE *file;
		BZFILE* bzip;
		unsigned long int index = 0;
		std::string working_dir;
};
