#include <iostream>
#include <librealsense2/rs.hpp>
#include <opencv2/core.hpp>
#include <legacy_loader.hpp>

//Plan: 
//       Legacy loader to simulate a realsense device (Make sure it inherits from realsense device...?!)
//       Real saving and loading using Bzip2 and raw format + header w/ info and extra space at beginning 
//       Wrapper class for OpenCV around realsense device of any kind

int main () {
	std::cout << "Hello, world!" << std::endl;
}
