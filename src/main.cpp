#include <iostream>
#include <librealsense2/rs.hpp>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <cv_rs_wrapper.hpp>
//#include <legacy_loader.hpp>
#include <raw_video.hpp>
#include <iostream>

int main (int argc, char** argv) {
	if (argc > 1) {

		cv::Size frame_size;

		CVRealsenseWrapper* wrap = 0;
		if (argc > 2) {
			frame_size = cv::Size(640, 480);
			wrap = new CVRealsenseWrapper (cv::Size(640, 480), frame_size, 30);
		}

		RawVideo raw (argv[1], argc > 2 ? RawVideo::Mode::Write : RawVideo::Mode::Read, frame_size);

		cv::Mat color_frame = cv::Mat::zeros(frame_size, CV_8UC3);
		cv::Mat depth_frame = cv::Mat::zeros(frame_size, CV_16UC1);

		do {
			if (wrap) {
				wrap->wait_for_frames(depth_frame, color_frame);
			}
			imshow("Depth", depth_frame);
			imshow("Color", color_frame);
		} while (cv::waitKey(0) != 'q' && raw.next(depth_frame, color_frame));
		return EXIT_SUCCESS;
	} else {
		std::cerr << "Usage: " << argv[0] << " <filename> <-c>" << std::endl;
		return EXIT_FAILURE;
	}
}
