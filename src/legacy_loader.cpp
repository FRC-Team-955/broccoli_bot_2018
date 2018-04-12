#include <legacy_loader.hpp>

LegacyLoader::LegacyLoader(char *file_name) {
	input_file = std::ifstream(file_name, std::ios::binary);
	input_file.read((char *)rgb_buffer, data_length_color);
	input_file.read((char *)depth_buffer, data_length_depth);

	auto depth_sensor = dev.add_sensor("Depth");
	auto color_sensor = dev.add_sensor("Color");

	rs2_intrinsics intrinsics = {
		frame_width,
		frame_height,
		(float)frame_width / 2,
		(float)frame_height / 2,
		(float)frame_width,
		(float)frame_height,
		RS2_DISTORTION_BROWN_CONRADY,
		{0, 0, 0, 0, 0}
	};

	auto depth_stream = depth_sensor.add_video_stream(
			{RS2_STREAM_DEPTH, 0, 0, frame_width, frame_height, 60, 2, RS2_FORMAT_Z16,
			intrinsics});

	depth_sensor.add_read_only_option(RS2_OPTION_DEPTH_UNITS, 0.001f);

	auto color_stream = color_sensor.add_video_stream(
			{RS2_STREAM_COLOR, 0, 1, frame_width, frame_height, 60, 3,
			RS2_FORMAT_RGBA8, intrinsics});

	dev.create_matcher(RS2_MATCHER_DLR_C);
	rs2::syncer sync;

	depth_sensor.open(depth_stream);
	color_sensor.open(color_stream);

	depth_sensor.start(sync);
	color_sensor.start(sync);
}
