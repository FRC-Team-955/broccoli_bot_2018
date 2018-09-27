#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <rgbd_frame_source.hpp>
#include <optional>

struct BroccoliVision {
    // Functions/Constructors
    BroccoliVision(int argc, char** argv);
    bool step(); // Should return false when no more processing can be done

    // Command line specifications
    char* frame_dir;
    char* config_dir;
    bool run_realtime;
    bool run_debug;
    bool run_headless;
    bool run_from_file;
    bool run_record_to_file;

    // Run-time variables
    RGBDFrameSource frame_source;
};

int main (int argc, char** argv) {
    BroccoliVision loop(argc, argv);
    while(loop.step());
    exit(EXIT_SUCCESS);
}

bool BroccoliVision::step() {
    
    return true;
}

// Read command line and set options
BroccoliVision::BroccoliVision(int argc, char** argv) {
    enum ArgType { play, record, debug } argument_num;
    static struct option long_options[] = {
        {"play",    required_argument,  NULL,  play    },
        {"record",  required_argument,  NULL,  record  },
        {"debug",   no_argument,        NULL,  debug   },
    };

    frame_dir           =  NULL;
    config_dir          =  NULL;
    run_realtime        =  true;
    run_debug           =  false;
    run_from_file       =  false;
    run_record_to_file  =  false;
    run_headless        =  true;

    int ret = 0;
    while ((ret = getopt_long(argc, argv, "", long_options, NULL)) != -1) {
        switch (ret) {
            case '?': exit(EXIT_FAILURE);
            case ArgType::play:
                      run_realtime = false;
                      run_from_file = true;
                      run_record_to_file = false;
                      frame_dir = optarg;
                      break;

            case ArgType::record:
                      run_realtime = true;
                      run_from_file = false;
                      run_record_to_file = true;
                      frame_dir = optarg;
                      break;

            case ArgType::debug: 
                      run_debug = true; 
                      run_headless = false; 
                      break;
        }
    }
    if (optind == argc) {
        printf("No config file specified. Exiting.\n");
        exit(EXIT_FAILURE);
    } else {
        config_dir = argv[optind];
    }
}
