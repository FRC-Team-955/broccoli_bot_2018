#pragma once

#include <getopt.h>
#include <bgrd_frame_source.hpp>
#include <realsense_bgrdfs.hpp>
#include <folder_bgrdfs.hpp>
#include <exception>

class BroccoliVision {
    public:
        // Functions/Constructors
        BroccoliVision(int argc, char** argv);
        ~BroccoliVision();
        bool step(); // Should return false when no more processing can be done

    private:
        // Command line specifications
        char*  frame_dir         =  NULL;
        char*  config_dir        =  NULL;
        char*  train_output_dir  =  NULL;

        bool  run_realtime           =  true;
        bool  run_debug              =  false;
        bool  run_from_file          =  false;
        bool  run_record_to_file     =  false;
        bool  run_headless           =  true;
        bool  run_output_train_data  =  false;

        // Run-time variables
        RGBDFrameSource* frame_source;
        FrameWriter* train_data_writer;
};
