#include <cv_rs_wrapper.hpp>
#include <iostream>
#include <librealsense2/rs.hpp>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
//#include <legacy_loader.hpp>
#include <iostream>
#include <raw_video.hpp>
#include <vector>
#include <chrono>

const char *trackbar_save_name = "config.yml";
const char *trackbar_win_name = "config";
using namespace cv;
int main(int argc, char **argv) {
	if (argc > 1) {

		typedef std::chrono::high_resolution_clock Clock;
		Point3i hsv_min(0, 0, 0);
		Point3i hsv_max(255, 255, 255);

		namedWindow(trackbar_win_name);

		{
			FileStorage fs;
			if (fs.open(trackbar_save_name,
						cv::FileStorage::FORMAT_YAML | cv::FileStorage::READ)) {
#define READ_ENT(NAME) fs[#NAME] >> NAME;
				READ_ENT(hsv_min);
				READ_ENT(hsv_max);
#undef READ_ENT
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

		CVRealsenseWrapper *wrap = 0;
		if (argc > 2) {
			frame_size = Size(640, 480);
			wrap = new CVRealsenseWrapper(Size(640, 480), frame_size, 30);
		}

		RawVideo raw(argv[1],
				argc > 2 ? RawVideo::Mode::Write : RawVideo::Mode::Read,
				frame_size);

		Mat color_frame = Mat::zeros(frame_size, CV_8UC3);
		Mat depth_frame = Mat::zeros(frame_size, CV_16UC1);
		Mat hsv;
		Mat hsv_mask;
		Mat laplacian_out;
		Mat correct_color_lap;
		Mat disp;
		Mat morph_kern = getStructuringElement(MORPH_ELLIPSE, Size(15, 15));

		std::vector<std::vector<Point>> contours;
		std::vector<Point> convex;

		bool run_app = true;
		bool pause = false;
		do {
			if (wrap) {
				wrap->wait_for_frames(depth_frame, color_frame);
			}
			cvtColor(color_frame, hsv, CV_BGR2HSV);

			inRange(hsv, Scalar(hsv_min.x, hsv_min.y, hsv_min.z),
					Scalar(hsv_max.x, hsv_max.y, hsv_max.z), hsv_mask);

			Laplacian(color_frame, laplacian_out, CV_8UC1);

			correct_color_lap.setTo(0);

			laplacian_out.copyTo(correct_color_lap, hsv_mask);

			inRange(correct_color_lap, Scalar(1, 1, 1), Scalar(255, 255, 255), disp);
			dilate(disp, disp, morph_kern);

			imshow("disp", disp);
			if (!pause && !raw.next(depth_frame, color_frame)) {
				raw.reset_index();
				run_app = false;
			}
			switch (waitKey(30)) {
				case 'q':
					run_app = false;
					break;
				case ' ':
					pause = !pause;
					break;
			}
		} while (run_app);

		FileStorage fs;
		if (fs.open(trackbar_save_name,
					cv::FileStorage::FORMAT_YAML | cv::FileStorage::WRITE)) {
#define WRITE_ENT(NAME) fs << #NAME << NAME;
			WRITE_ENT(hsv_min);
			WRITE_ENT(hsv_max);
#undef WRITE_ENT
		} else {
			std::cerr << "Error writing " << trackbar_save_name << std::endl;
		}
		return EXIT_SUCCESS;
	} else {
		std::cerr << "Usage: " << argv[0] << " <filename> <-c>" << std::endl;
		return EXIT_FAILURE;
	}
}
