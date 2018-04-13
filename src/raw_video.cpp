#include <raw_video.hpp>

RawVideo::RawVideo(char* file_name, Mode mode, cv::Size& frame_size) : mode(mode) {
	int bzerror;
	file = fopen ( "myfile.bz2", mode == Mode::Write ? "w" : "r" );
	if (!file) {
		std::cerr << "ERROR opening file" << std::endl;
	}
	Header init_header;
	if (mode == Mode::Write) {
		bzip = BZ2_bzWriteOpen( &bzerror, file, 0, 0, 0 );
		if (!bzip) {
			BZ2_bzWriteClose ( &bzerror, bzip, 0, NULL, NULL );
		}
		init_header.frame_height = frame_size.height;	
		init_header.frame_width = frame_size.width;	
		BZ2_bzWrite(&bzerror, bzip, &init_header, sizeof(Header));
	} else {
		bzip = BZ2_bzReadOpen( &bzerror, file, 0, 0, NULL, 0 );
		if (!bzip) {
			BZ2_bzReadClose ( &bzerror, bzip );
		}
		BZ2_bzRead(&bzerror, bzip, &init_header, sizeof(Header));
		frame_size.height = init_header.frame_height;	
		frame_size.width = init_header.frame_width;	
	}
}

bool RawVideo::next(cv::Mat& depth_frame, cv::Mat& color_frame) {
	int bzerror;
#define OP(NAME, OPER) BZ2_bz##OPER(&bzerror, bzip, NAME.data, NAME.total() * NAME.elemSize());
	if (mode == Mode::Write) {
		OP(depth_frame, Write);	
		OP(color_frame, Write);	
	} else {
		OP(depth_frame, Read);	
		OP(color_frame, Read);	
	}
#undef OP
	return bzerror == BZ_OK || bzerror == BZ_STREAM_END;
}

RawVideo::~RawVideo() {
	int bzerror;
	if (mode == Mode::Write) {
		BZ2_bzWriteClose ( &bzerror, bzip, 0, NULL, NULL );
	} else {
		BZ2_bzReadClose ( &bzerror, bzip );
	}
}

void RawVideo::Header::print() {
	std::cout << "W: " << frame_width << " H: " << frame_height << " Ver: " << version << std::endl;
}
