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

const char *trackbar_save_name = "config.yml";
const char *trackbar_win_name = "config";
using namespace cv;

int main(int argc, char **argv) {
	if (argc > 1) {

#define PARAM(NAME) int NAME;
#include <saving.h>
#undef PARAM

		namedWindow(trackbar_win_name);

		{
			FileStorage fs;
			if (fs.open(trackbar_save_name, cv::FileStorage::FORMAT_YAML | cv::FileStorage::READ)) {
#define PARAM(NAME) fs[#NAME] >> NAME;
#include <saving.h>
#undef PARAM
			} else {
				std::cerr << "Error reading " << trackbar_save_name << std::endl;
			}
		}

#define PARAM(NAME) createTrackbar(#NAME, trackbar_win_name, &NAME, 255);
#include <saving.h>
#undef PARAM

		Size frame_size;

		CVRealsenseWrapper *wrap = 0;
		if (argc > 2) {
			frame_size = Size(640, 480);
			wrap = new CVRealsenseWrapper(Size(640, 480), frame_size, 30);
		}

		RawVideo raw(argv[1], argc > 2 ? RawVideo::Mode::Write : RawVideo::Mode::Read, frame_size);

		Mat color_frame_sz = Mat::zeros(frame_size, CV_8UC3);
		Mat depth_frame_sz = Mat::zeros(frame_size, CV_16UC1);
		Mat color_frame;
		Mat depth_frame;
		Mat hsv;
		Mat hsv_mask;
		Mat laplacian_out;
		Mat correct_color_lap;
		Mat morph_out;
		Mat colors[3];
		Mat morph_kern = getStructuringElement(MORPH_ELLIPSE, Size(7, 7));

		std::vector<std::vector<Point>> contours;

		float downsample = 2.0;

		bool run_app = true;
		bool pause = false;
		do {
			if (wrap) {
				wrap->wait_for_frames(depth_frame_sz, color_frame_sz);
			}
			resize(color_frame_sz, color_frame, Size(), 1.0 / downsample, 1.0 / downsample);
			split(color_frame, colors);
			/*
			resize(depth_frame_sz, depth_frame, Size(), 0.25, 0.25);
			cvtColor(color_frame, hsv, CV_BGR2HSV);
			correct_color_lap.setTo(0);
			inRange(hsv, Scalar(hue_min, sat_min, val_min), Scalar(hue_max, sat_max, val_max), hsv_mask);
			laplacian_out.copyTo(correct_color_lap, hsv_mask);
			*/

			Laplacian(colors[1], laplacian_out, CV_8UC1, 1);
			inRange(laplacian_out, Scalar(is_broc_thresh), 255, laplacian_out);
			imshow("lap", laplacian_out);
			std::chrono::high_resolution_clock::time_point t1 = std::chrono::high_resolution_clock::now();
			//Mat nacced = nac(laplacian_out, smooth + 1, (float)thresh / 16.0);

			dilate(laplacian_out, morph_out, morph_kern);
			erode(morph_out, morph_out, morph_kern);
			std::chrono::high_resolution_clock::time_point t2 = std::chrono::high_resolution_clock::now();
    		auto duration = std::chrono::duration_cast<std::chrono::microseconds>( t2 - t1 ).count();
			std::cout << duration << std::endl;
			
			/*
			findContours(nacced, contours, CV_RETR_LIST, CV_CHAIN_APPROX_SIMPLE);
			Mat final;	
			color_frame.copyTo(final);
			for (int i = 0; i < contours.size(); i++) {
				std::vector<Point> hull;
				convexHull(contours[i], hull);
				if (contourArea(hull) > area_thresh * area_thresh * area_thresh) {
					polylines(final, hull, true, Scalar(255, 0, 204), 2);
					Point accum;
					for (auto& pt : hull)
						accum += pt;
					accum.x /= hull.size();
					accum.y /= hull.size();
					circle(final, accum, 4, Scalar(255, 255, 0));
				}
			}
			*/

			imshow("morph_out", morph_out);
			//imshow("hsv", hsv_mask);
			//imshow("color", (colors[1] * 3) - colors[0] - colors[2]);
			//imshow("nac", nacced);
			//imshow("color", final);
			if (!pause && !raw.next(color_frame_sz, depth_frame_sz)) {
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
				case 'a':
					imwrite("/home/duncan/cap.png", laplacian_out);
					break;
			}
		} while (run_app);

		FileStorage fs;
		if (fs.open(trackbar_save_name,
					cv::FileStorage::FORMAT_YAML | cv::FileStorage::WRITE)) {
#define PARAM(NAME) fs << #NAME << NAME;
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
