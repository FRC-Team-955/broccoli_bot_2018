#include <opencv2/core.hpp>
#include <rgbd_frame.hpp>

class RGBDFrameSource {
    virtual RGBDFrame next();
};
