#include <iostream>
#include <broccoli_vision.hpp>

int main (int argc, char** argv) {
    try {
        BroccoliVision loop(argc, argv);
        std::cout << "Initialized. Starting main loop." << std::endl;
        while(loop.step());
        std::cout << "Safe exit. \e[92mGoodbye!\e(B\e[m" << std::endl;
        return EXIT_SUCCESS;
    } catch (const std::exception& e) {
        std::cerr << "\e[31mFatal error: \e(B\e[m" << e.what() << std::endl;
        return EXIT_FAILURE;
    }
}
