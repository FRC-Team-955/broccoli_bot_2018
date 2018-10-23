#pragma once

#include <opencv2/opencv.hpp>
#include <dirent.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

class FrameDirectory {
    public:
        FrameDirectory(char* directory, char* format, bool create_if_nonexistant);
        char* next_name();
        void advance_to_last();

    private:
        unsigned int frame_index = 0;
        char buf[512];
        char* name_format;
        char* directory;
};

class FrameWriter {
    public:
        FrameWriter(char* directory, char* name_format);
        void next(cv::Mat input);

    private:
        FrameDirectory frame_name;
};

class FrameReader {
    public:
        FrameReader(char* directory, char* name_format);
        cv::Mat next();

    private:
        FrameDirectory frame_name;
};
