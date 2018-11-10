#include <iostream>
#include <opencv2/opencv.hpp>
#include <bgrd_frame_source.hpp>
#include <folder_bgrdfs.hpp>
#include <declarative_broccoli_locator.hpp>
#include <declarative_broccoli_locator_visuals.hpp>

int main (int argc, char** argv) {
    char* frame_dir = (char*)"../datasets/record_1";
    char* config_dir = (char*)"config.yml";
    FolderBGRDFrameSource source (frame_dir);

    ObjectLocator* locator;
    { 
        cv::FileStorage fs(config_dir, CV_STORAGE_FORMAT_YAML | CV_STORAGE_READ);
        locator = new DeclarativeBroccoliLocator(fs);
        fs.release();
    }

    DeclarativeBroccoliLocator* decl_broc_locator_cast = dynamic_cast<DeclarativeBroccoliLocator*>(locator); 
    if (decl_broc_locator_cast) DeclarativeBroccoliLocatorVisuals::init_sliders(*decl_broc_locator_cast, "Settings");

    cv::Mat display_frame;
    bool run = true;
    bool paused = false;
    BGRDFrame frameset = BGRDFrame(cv::Mat(), cv::Mat());

    while (run) {
        if (!paused) frameset = source.next();
        if (frameset.bgr.empty()) break;

        frameset.bgr.copyTo(display_frame);

        cv::Rect rect = locator->locate(frameset.bgr);
        if (decl_broc_locator_cast) DeclarativeBroccoliLocatorVisuals::show_internals(*decl_broc_locator_cast);
        if (rect.area() > 0) cv::rectangle(display_frame, rect, cv::Scalar(255, 0, 128), 1);

        //int roi_squeeze_px = (float(roi_squeeze) / 255.0) * (frame_size.height / 2);
        //Rect roi (roi_squeeze_px, 0, frame_size.width - (2 * roi_squeeze_px), frame_size.height);

        //imshow("display", display_frame);

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
    fs.release();

}
