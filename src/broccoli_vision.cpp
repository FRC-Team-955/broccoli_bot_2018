#include <broccoli_vision.hpp>

bool BroccoliVision::step() {
    std::optional<RGBDFrame> frame_combined = frame_source->next(); 
    if (!frame_combined.has_value()) { 
        throw std::runtime_error("Missing frame!");
        return false;
    }
    cv::Mat& depth = frame_combined.value().depth;
    cv::Mat& bgr = frame_combined.value().bgr;

    if (run_debug) {
        cv::imshow ("depth", depth);
        cv::imshow ("color", bgr);
        if (cv::waitKey(1) == 'q') return false;
    }

    return true;
}

// Read command line and set options
BroccoliVision::BroccoliVision(int argc, char** argv) {
    // Getopt setup
    enum ArgType { play, record, debug, train_data } argument_num;
    static struct option long_options[] = {
        {"play",        required_argument,  NULL,  play        },
        {"record",      required_argument,  NULL,  record      },
        {"train_data",  required_argument,  NULL,  train_data  },
        {"debug",       no_argument,        NULL,  debug       },
    };

    // Parse each option and decide what to change
    int ret = 0;
    while ((ret = getopt_long(argc, argv, "", long_options, NULL)) != -1) {
        switch (ret) {
            case '?': throw std::runtime_error("Invalid argument(s)"); break;
            case ArgType::play:
                      run_realtime        =  false;
                      run_record_to_file  =  false;
                      run_from_file       =  true;
                      frame_dir = optarg;
                      break;

            case ArgType::record:
                      run_realtime        =  true;
                      run_record_to_file  =  true;
                      run_from_file       =  false;
                      frame_dir = optarg;
                      break;

            case ArgType::debug: 
                      run_debug = true; 
                      run_headless = false; 
                      break;

            case ArgType::train_data:
                      run_output_train_data = true;
                      train_output_dir = optarg;
                      break;
        }
    }
    if (optind == argc) {
        throw std::runtime_error("No config file specified");
    } else {
        // Set config dir to the first word after all command line options are finished
        config_dir = argv[optind];
    }

    if (run_realtime) {
        throw std::runtime_error("Unimplemented!");
        //this->frame_source = new RealSenseRGBDFrameSource(cv::Size(1920, 1080), 30);
    } else {
        this->frame_source = new FolderRGBDFrameSource(frame_dir);
    }
}

BroccoliVision::~BroccoliVision() {
}
