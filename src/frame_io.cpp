#include <frame_io.hpp>

FrameDirectory::FrameDirectory(char* directory, char* name_format, bool create_if_nonexistant) : directory(directory), name_format(name_format) {
    if (closedir(opendir(directory)) == -1) { //Check if the dir does not exist
        if (create_if_nonexistant) {
            if (mkdir(directory, 0755) == -1) { //If failed on create dir
                throw std::runtime_error("Cannot create frame writing directory.");
            }
        } else {
            throw std::runtime_error("Frame directory '" + std::string(directory) + "' does not exist.");
        }
    }
}

char* FrameDirectory::next_name() {
    sprintf(buf, name_format, directory, frame_index++);
    return buf;
}

void FrameDirectory::advance_to_last() {
    while (access(next_name(), F_OK) != -1);
}

FrameWriter::FrameWriter(char* directory, char* name_format) : frame_name(directory, name_format, true)  {
    frame_name.advance_to_last();
}

void FrameWriter::next(cv::Mat input) {
    cv::imwrite(frame_name.next_name(), input);
}

FrameReader::FrameReader(char* directory, char* name_format) : frame_name(directory, name_format, false) {}

cv::Mat FrameReader::next() {
    return cv::imread(frame_name.next_name(), cv::IMREAD_UNCHANGED);
}
