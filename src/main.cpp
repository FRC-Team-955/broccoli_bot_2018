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

const char *trackbar_save_name = "config.yml";
const char *trackbar_win_name = "config";
using namespace cv;
int main(int argc, char **argv) {
	if (argc > 1) {

		Point3i hsv_min(0, 0, 0);
		Point3i hsv_max(255, 255, 255);
		int blur_size = 0;
		int is_broc_thresh = 0;

		namedWindow(trackbar_win_name);

		{
			FileStorage fs;
			if (fs.open(trackbar_save_name,
						cv::FileStorage::FORMAT_YAML | cv::FileStorage::READ)) {
#define READ_ENT(NAME) fs[#NAME] >> NAME;
				READ_ENT(hsv_min);
				READ_ENT(hsv_max);
				READ_ENT(blur_size);
				READ_ENT(is_broc_thresh);
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

		createTrackbar("is_broc_thresh", trackbar_win_name, &is_broc_thresh, 255);
		createTrackbar("blur_size", trackbar_win_name, &blur_size, 255);

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
		Mat lap_blur;
		Mat correct_color_lap;
		Mat is_broc;
		Mat is_broc_canny;
		Mat color_visual;

		std::vector<std::vector<Point>> contours;
		std::vector<Point> convex;

		bool run_app = true;
		bool pause = false;
		do {
			if (wrap) {
				wrap->wait_for_frames(depth_frame, color_frame);
			}
			color_frame.copyTo(color_visual);
			cvtColor(color_frame, hsv, CV_BGR2HSV);

			inRange(hsv, Scalar(hsv_min.x, hsv_min.y, hsv_min.z),
					Scalar(hsv_max.x, hsv_max.y, hsv_max.z), hsv_mask);

			Laplacian(color_frame, laplacian_out, CV_8UC1);

			correct_color_lap.setTo(0);

			laplacian_out.copyTo(correct_color_lap, hsv_mask);

			blur(correct_color_lap, lap_blur,
					cv::Size(1 + blur_size * 2, 1 + blur_size * 2));

			inRange(lap_blur,
					Scalar(is_broc_thresh, is_broc_thresh, is_broc_thresh),
					Scalar(255, 255, 255), is_broc);

			Canny(is_broc, is_broc_canny, 128, 128);
			//Rect bound (1, 1, is_broc_canny.rows - 1, is_broc_canny.cols - 1);
			//rectangle(is_broc_canny, bound, Scalar(0,0,0));

			findContours(is_broc_canny, contours, CV_RETR_EXTERNAL,
					CV_CHAIN_APPROX_SIMPLE);

			for (int i = 0; i < contours.size(); i++) {
				convexHull(contours[i], convex, true);
				//polylines(color_visual, convex, true, Scalar(255, 0, 255), 2);
				fillConvexPoly(color_visual, convex, Scalar(0, 128, 255));
				//drawContours(color_visual, convex, 0, Scalar(0, 255, 0));
			}

			// imshow("Depth", depth_frame * 10);
			// imshow("Color", color_frame);
			// imshow("hsv", hsv_mask);
			imshow("out", color_visual);
			imshow("cann", is_broc_canny);
			// imshow("lap", correct_color_lap * 5);
			// imshow("lap_b", lap_blur * 15);
			if (!pause && !raw.next(depth_frame, color_frame)) {
				raw.reset_index();
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
			WRITE_ENT(is_broc_thresh);
			WRITE_ENT(blur_size);
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
