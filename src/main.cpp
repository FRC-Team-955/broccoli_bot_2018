#include <iostream>
#include <opencv2/opencv.hpp>
#include <bgrd_frame_source.hpp>
#include <folder_bgrdfs.hpp>
#include <declarative_broccoli_locator.hpp>
#include <declarative_broccoli_locator_visuals.hpp>
#include <histogram.hpp>
#include <iostream>

int main (int argc, char** argv) {
    char* frame_dir = (char*)"../datasets/record_1";
    char* config_dir = (char*)"../config.yml";
    FolderBGRDFrameSource source (frame_dir);
    int width_reduction = 0;
    int min_hist = 10;
    int max_hist = 3000;
    float percentile = 25.0 / 100.0;

    ObjectLocator* locator;
    { 
        cv::FileStorage fs(config_dir, CV_STORAGE_FORMAT_YAML | CV_STORAGE_READ);
        locator = new DeclarativeBroccoliLocator(fs);
        fs["width_reduction"] >> width_reduction;
        fs.release();
    }

    // Redudant for now, but will become relevant with other crops and detection methods.
    DeclarativeBroccoliLocator* decl_broc_locator_cast = dynamic_cast<DeclarativeBroccoliLocator*>(locator); 
    if (decl_broc_locator_cast) DeclarativeBroccoliLocatorVisuals::init_sliders(*decl_broc_locator_cast, "Settings");
    cv::createTrackbar("Width reduction", "Settings", &width_reduction, 1080/2);

    auto hist = Histogram<unsigned short>(min_hist, max_hist);

    cv::Mat display_frame;
    bool run = true;
    bool paused = false;
    BGRDFrame frameset = BGRDFrame(cv::Mat(), cv::Mat());

    while (run) {
        if (!paused) frameset = source.next().reduce_width(width_reduction);
        if (frameset.bgr.empty()) break;

        frameset.bgr.copyTo(display_frame);

        cv::Rect rect = locator->locate(frameset.bgr);
        if (!!decl_broc_locator_cast) {
            DeclarativeBroccoliLocatorVisuals::show_internals(*decl_broc_locator_cast);
            DeclarativeBroccoliLocatorVisuals::draw_contours(*decl_broc_locator_cast, display_frame, cv::Scalar(128, 0, 255));
        }
        if (rect.area() > 0) {
            cv::rectangle(display_frame, rect, cv::Scalar(0, 255, 128), 1);
            cv::Mat sample = frameset.depth(rect);
            hist.clear();
            hist.insert_image(sample);
            std::cout << hist.take_percentile(percentile) << std::endl;
        }

        imshow("display_frame", display_frame);
        switch (cv::waitKey(1) & 0xFF) {
            case 'q':
                run = false;
                break;
            case ' ':
                paused = !paused;
                break;
        }
    }

    // Save and quit
    cv::FileStorage fs(config_dir, CV_STORAGE_FORMAT_YAML | CV_STORAGE_WRITE);
    locator->save_parameters(fs);
    fs << "width_reduction" << width_reduction;
    fs.release();

    return EXIT_SUCCESS;
}
