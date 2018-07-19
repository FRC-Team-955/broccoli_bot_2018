#include <cv_rs_wrapper.hpp>
#include <iostream>
#include <librealsense2/rs.hpp>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <legacy_loader.hpp>
#include <iostream>
#include <raw_video.hpp>
#include <vector>
#include <string.h>
#include <stdlib.h>
#include <chrono>
#include <stdio.h>

#define DISP(MAT) imshow(#MAT, MAT);

//TODO:
//	Add 'begin at' and 'start paused' argument options

const char* trackbar_save_name = "config.yml"; //TODO: turn this into an argument
const char* trackbar_win_name = "config";
using namespace cv;

int main(int argc, char **argv) {
	if (argc > 1) {
		bool is_live = argc > 2;
#define PARAM(NAME, TRACKBAR) int NAME;
#include <saving.h>
#undef PARAM
		namedWindow(trackbar_win_name);

		{
			FileStorage fs;
			if (fs.open(trackbar_save_name, cv::FileStorage::FORMAT_YAML | cv::FileStorage::READ)) {
#define PARAM(NAME, TRACKBAR) fs[#NAME] >> NAME;
#include <saving.h>
#undef PARAM
			} else {
				std::cerr << "Error reading " << trackbar_save_name << std::endl;
			}
		}

#define PARAM(NAME, TRACKBAR) if (TRACKBAR) createTrackbar(#NAME, trackbar_win_name, &NAME, 255);
#include <saving.h>
#undef PARAM

		Mat morph_element = getStructuringElement (0, Size (2 * morph_size + 1, 2 * morph_size + 1));
		Mat morph_tiny = getStructuringElement (0, Size (1,1));

		Size frame_size;
		
		frame_size = Size(640, 480);
		CVRealsenseWrapper *wrap = 0;
		if (is_live) {
			wrap = new CVRealsenseWrapper(frame_size, frame_size, 30);
		}

		RawVideo raw(argv[1], is_live ? RawVideo::Mode::Write : RawVideo::Mode::Read, frame_size);

		Mat color_frame = cv::Mat::zeros(frame_size, CV_8UC3);
		Mat depth_frame = cv::Mat::zeros(frame_size, CV_16UC1);

		Mat depth_frame_roi, color_frame_roi;
		Mat color_hsv, hsv_mask;
		Mat color_split[3];
		Mat laplacian, laplacian_confirmed;
		Mat lap_conf_mask_combined;
		Mat morph_blob;
		Mat canny_edges;
		Mat large_blob_mask;
		Mat display_frame;

		std::vector<std::vector<Point>> contours;

		std::chrono::high_resolution_clock::time_point tp_begin;
		std::chrono::high_resolution_clock::time_point tp_capture;
		std::chrono::high_resolution_clock::time_point tp_process;

		bool run_app = true;
		bool pause = false;
		do {
			tp_begin = std::chrono::high_resolution_clock::now();

			if (wrap) {
				wrap->wait_for_frames(depth_frame, color_frame);
			}

			if (!pause && !raw.next(color_frame, depth_frame)) {
				raw.reset_index();
				return 0;
			}
			switch (waitKey(30)) {
				case 'q':
					run_app = false;
					break;
				case ' ':
					pause = !pause;
					break;
			}

			tp_capture = std::chrono::high_resolution_clock::now();

			int roi_squeeze_px = (float(roi_squeeze) / 255.0) * (frame_size.height / 2);
			Rect roi (roi_squeeze_px, 0, frame_size.height - (2 * roi_squeeze_px), frame_size.width);

			color_frame_roi = color_frame(roi);
			depth_frame_roi = depth_frame(roi);
			color_frame_roi.copyTo(display_frame);

			//DISP(depth_frame_roi);

			cvtColor(color_frame_roi, color_hsv, CV_RGB2HSV);
			inRange(color_hsv, Scalar(hue_min, sat_min, val_min), Scalar(hue_max, sat_max, val_max), hsv_mask);
			DISP(hsv_mask);

			split(color_frame_roi, color_split);
			Laplacian(color_split[1], laplacian, CV_8UC1);
			inRange(laplacian, Scalar(lap_thresh), Scalar(255), laplacian_confirmed);
			//DISP(laplacian_confirmed);

			bitwise_and(laplacian_confirmed, hsv_mask, lap_conf_mask_combined);
			//DISP(lap_conf_mask_combined);

			morphologyEx(lap_conf_mask_combined, morph_blob, MORPH_CLOSE, morph_element);
			morphologyEx(morph_blob, morph_blob, MORPH_OPEN, morph_element);
			//DISP(morph_blob);

			Canny(morph_blob, canny_edges, 128, 128);
			DISP(canny_edges);

			findContours(canny_edges, contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_TC89_L1);

			for (auto& cont : contours) cont[cont.size() - 1] = cont[0];

			unsigned int largest = 0;
			int largest_index = -1;
			for (int i = 0; i < contours.size(); i++) {
				unsigned int area = boundingRect(contours[i]).area();
				if (area > largest && area > area_thresh * area_thresh) {
					largest_index = i;
					largest = area;
				}
			}
			drawContours(display_frame, contours, -1, Scalar(255, 128, 0), 2);//CV_FILLED);

			if (largest_index != -1) {
				Rect broccoli_roi = boundingRect(contours[largest_index]);
				rectangle(display_frame, broccoli_roi, Scalar(0, 0, 255), 2);
				printf("%f\n", mean(depth_frame_roi(broccoli_roi), morph_blob(broccoli_roi))[0]);
			}

			DISP(display_frame);

			tp_process = std::chrono::high_resolution_clock::now();
			std::cout << "Capture time: " << std::chrono::duration_cast<std::chrono::milliseconds>(tp_capture - tp_begin).count() << "ms" << std::endl;
			std::cout << "Process time: " << std::chrono::duration_cast<std::chrono::milliseconds>(tp_process - tp_capture).count() << "ms" << std::endl;
			std::cout << std::endl;

		} while (run_app);

		FileStorage fs;
		if (fs.open(trackbar_save_name,
					cv::FileStorage::FORMAT_YAML | cv::FileStorage::WRITE)) {
#define PARAM(NAME, TRACKBAR) fs << #NAME << NAME;
#include <saving.h>
#undef PARAM
		} else {
			std::cerr << "Error writing " << trackbar_save_name << std::endl;
		}
		return EXIT_SUCCESS;
	} else {
		std::cerr << "Usage: " << argv[0] << " <filename> <-c>" << std::endl;
		return EXIT_FAILURE;
	}
}
#undef MAT
