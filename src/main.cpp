#include <iostream>
#include <opencv2/opencv.hpp>
#include <bgrd_frame_source.hpp>
#include <folder_bgrdfs.hpp>
#include <declarative_broccoli_locator.hpp>
#include <declarative_broccoli_locator_visuals.hpp>
#include <histogram.hpp>
#include <histogram_visuals.hpp>
#include <iostream>

#include <chrono>

int main (int argc, char** argv) {
    // Command line arguments
    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " <config.yml> (<dataset_dir>) (--show_visuals)" << std::endl;
        return EXIT_FAILURE;
    }
    char* config_dir = argv[1];
    bool reading_from_folder = argc >= 3;
    char* frame_dir = reading_from_folder ? argv[2] : (char*)"INVALID";
    bool show_visuals = argc == 4;

    // Generic image processing elements
    BGRDFrameSource* source;
    ObjectLocator* locator;

    // Runtime image processing variables
    int width_reduction = 0;
    int min_hist = 1000;
    int max_hist = 4000;
    float percentile = 50.0 / 100.0;
    cv::Mat display_frame;
    BGRDFrame frameset = BGRDFrame(cv::Mat(), cv::Mat());

    // Read config, set up locator
    { 
        cv::FileStorage fs(config_dir, CV_STORAGE_FORMAT_YAML | CV_STORAGE_READ);
        locator = new DeclarativeBroccoliLocator(fs);
        fs["width_reduction"] >> width_reduction;
        fs["min_hist"] >> min_hist;
        fs["max_hist"] >> max_hist;
        fs.release();
    }

    // Redudant for now, but will become relevant with other crops and detection methods.
    DeclarativeBroccoliLocator* decl_broc_locator_cast = dynamic_cast<DeclarativeBroccoliLocator*>(locator); 

    // Choose frame source
    if (reading_from_folder) {
        source = new FolderBGRDFrameSource (argv[2]);
    } else {
        std::cerr << "Unimplemented!" << std::endl;
        return EXIT_FAILURE;
    }

    // Create UI primitives
    if (show_visuals) {
        if (decl_broc_locator_cast) DeclarativeBroccoliLocatorVisuals::init_sliders(*decl_broc_locator_cast, "Settings");
        cv::createTrackbar("Width reduction", "Settings", &width_reduction, 1080/2);
    }

    // Histogram, used for depth sampling
    auto depth_hist = Histogram<unsigned short>(min_hist, max_hist);

    // Runtime loop control vars
    bool run = true;
    bool paused = false;

    while (run) {
        // Retreive frames
        if (!paused) frameset = source->next().reduce_width(width_reduction);
        if (frameset.bgr.empty()) break; // Abort early if there is no data

        // Set up display frame
        if (show_visuals) frameset.bgr.copyTo(display_frame);

        std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();

        cv::Rect roi = locator->locate(frameset.bgr);

        std::chrono::steady_clock::time_point locate = std::chrono::steady_clock::now();

        // Broccoli is detected
        if (roi.area() > 0) {
            cv::Mat sample = frameset.depth(roi);
            depth_hist.clear();
            depth_hist.insert_image(sample);

            std::cout << depth_hist.take_percentile(percentile) << std::endl;
        }

        std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();

        std::cout << "Locate time: " << std::chrono::duration_cast<std::chrono::milliseconds>(locate - begin).count() << "ms ";        // Find broccoli
        std::cout << "Hist time: " << std::chrono::duration_cast<std::chrono::milliseconds>(end - locate).count() << "ms" <<std::endl;        // Find broccoli
        
        // Handle UI
        if (show_visuals) {
            // Declarative broccoli detector ONLY
            if (!!decl_broc_locator_cast) {
                DeclarativeBroccoliLocatorVisuals::show_internals(*decl_broc_locator_cast);
                DeclarativeBroccoliLocatorVisuals::draw_contours(*decl_broc_locator_cast, display_frame, cv::Scalar(128, 0, 255));
            }

            // Display a box around the ROI
            cv::rectangle(display_frame, roi, cv::Scalar(0, 255, 128), 1);
            imshow("display_frame", display_frame);
            //HistogramVisuals<unsigned short>::show_internals(depth_hist, "histogram");

            // Handle UI input, display callbacks
            switch (cv::waitKey(1) & 0xFF) {
                case 'q':
                    run = false;
                    break;
                case ' ':
                    paused = !paused;
                    break;
            }
        }
    }

    // Save and quit
    {
        cv::FileStorage fs(config_dir, CV_STORAGE_FORMAT_YAML | CV_STORAGE_WRITE);
        locator->save_parameters(fs);
        fs << "width_reduction" << width_reduction;
        fs << "min_hist" << min_hist;
        fs << "max_hist" << max_hist;
        fs.release();
    }
    return EXIT_SUCCESS;
}
