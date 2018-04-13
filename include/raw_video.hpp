#pragma once

#include <opencv2/core.hpp>
#include <sys/stat.h>
#include <bzlib.h>
#include <iostream>

class RawVideo {
	public:
		enum Mode {
			Read,
			Write,
		};
		RawVideo(char* file_name, Mode mode, cv::Size& frame_size);
		bool next(cv::Mat& color_frame, cv::Mat& depth_frame); //Either read or write frames depending on mode
		~RawVideo();
		struct Header {
			uint32_t version;
			uint32_t frame_width;
			uint32_t frame_height;
			void print();
		};
	private:
		Mode mode;
		FILE *file;
		BZFILE* bzip;
};
