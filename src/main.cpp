#include <iostream>
#include <librealsense2/rs.hpp>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <cv_rs_wrapper.hpp>
//#include <legacy_loader.hpp>
#include <raw_video.hpp>
#include <iostream>

const char* trackbar_save_name = "config.yml";
const char* trackbar_win_name = "config";
using namespace cv;
int main (int argc, char** argv) {
	if (argc > 1) {

		Point3i hsv_min(0, 0, 0);
		Point3i hsv_max(255, 255, 255);

		namedWindow(trackbar_win_name);

		{
			FileStorage fs;
			if (fs.open(trackbar_save_name, cv::FileStorage::FORMAT_YAML | cv::FileStorage::READ)) {
				fs["hsv_min"] >> hsv_min;
				fs["hsv_max"] >> hsv_max;
			} else {
				std::cerr << "Error reading " << trackbar_save_name << std::endl;
			}
		}

		createTrackbar("hsv_min_hue", trackbar_win_name, &hsv_min.x, 255);
		createTrackbar("hsv_min_sat", trackbar_win_name, &hsv_min.y, 255);
		createTrackbar("hsv_min_val", trackbar_win_name, &hsv_min.z, 255);

		createTrackbar("hsv_max_hue", trackbar_win_name, &hsv_max.x, 255);
		createTrackbar("hsv_max_sat", trackbar_win_name, &hsv_max.y, 255);
		createTrackbar("hsv_max_val", trackbar_win_name, &hsv_max.z, 255);

		Size frame_size;

		CVRealsenseWrapper* wrap = 0;
		if (argc > 2) {
			frame_size = Size(640, 480);
			wrap = new CVRealsenseWrapper (Size(640, 480), frame_size, 30);
		}

		RawVideo raw (argv[1], argc > 2 ? RawVideo::Mode::Write : RawVideo::Mode::Read, frame_size);

		Mat color_frame = Mat::zeros(frame_size, CV_8UC3);
		Mat depth_frame = Mat::zeros(frame_size, CV_16UC1);
		Mat hsv;
		Mat correct_color;

		bool run_app = true;
		bool pause = false;
		do {
			if (wrap) {
				wrap->wait_for_frames(depth_frame, color_frame);
			}
			cvtColor(color_frame, hsv, CV_BGR2HSV);
			inRange(hsv, Scalar(hsv_min.x, hsv_min.y, hsv_min.z), Scalar(hsv_max.x, hsv_max.y, hsv_max.z), correct_color);
			//Laplacian(color_frame, out, CV_8UC1);
			//imshow("Depth", depth_frame * 10);
			//imshow("Color", color_frame);
			imshow("Out", correct_color);
			if (!pause && !raw.next(depth_frame, color_frame)) {
				raw.reset_index();
			}
			switch (waitKey(15)) {
				case 'q':
					run_app = false;
					break;
				case ' ':
					pause = !pause;
					break;
			}
		} while (run_app);

		FileStorage fs;
		if (fs.open(trackbar_save_name, cv::FileStorage::FORMAT_YAML | cv::FileStorage::WRITE)) {
			fs << "hsv_min" << hsv_min;
			fs << "hsv_max" << hsv_max;
		} else {
			std::cerr << "Error writing " << trackbar_save_name << std::endl;
		}
		return EXIT_SUCCESS;
	} else {
		std::cerr << "Usage: " << argv[0] << " <filename> <-c>" << std::endl;
		return EXIT_FAILURE;
	}
}
