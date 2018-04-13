#include <cv_rs_wrapper.hpp>

CVRealsenseWrapper::CVRealsenseWrapper(cv::Size depth_size, cv::Size color_size, int framerate) : align(RS2_STREAM_COLOR){
	rs2::config cfg;
	cfg.enable_stream(RS2_STREAM_DEPTH, depth_size.width, depth_size.height, RS2_FORMAT_Z16, framerate);
	cfg.enable_stream(RS2_STREAM_COLOR, color_size.width, color_size.height, RS2_FORMAT_BGR8, framerate);
	pipe.start(cfg);

	rs2::device dev = pipe.get_active_profile().get_device();
	for (rs2::sensor &sensor : dev.query_sensors()) {
		if (rs2::depth_sensor dpt = sensor.as<rs2::depth_sensor>()) {
			depth_scaling_factor = dpt.get_depth_scale();
			break;
		}
	}

	rs2::frameset data = align.process(pipe.wait_for_frames());
	rs2::frame depth = data.get_depth_frame();
	rs2::frame color = data.get_color_frame();

}

void CVRealsenseWrapper::wait_for_frames(cv::Mat& depth_frame, cv::Mat& color_frame) {
	rs2::frameset data = align.process(pipe.wait_for_frames());
	memcpy(depth_frame.data, (uchar*)data.get_depth_frame().get_data(), depth_frame.elemSize() * depth_frame.total());
	memcpy(color_frame.data, (uchar*)data.get_color_frame().get_data(), color_frame.elemSize() * color_frame.total());
}
