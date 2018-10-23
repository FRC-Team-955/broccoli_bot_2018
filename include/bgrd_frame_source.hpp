#pragma once

#include <opencv2/core.hpp>
#include <bgrd_frame.hpp>
#include <optional>

class RGBDFrameSource {
    public: 
        virtual std::optional<RGBDFrame> next() = 0;
};
