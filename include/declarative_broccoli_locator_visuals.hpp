#pragma once
#include <declarative_broccoli_locator.hpp>

class DeclarativeBroccoliLocatorVisuals {
    public:
        static void init_sliders(DeclarativeBroccoliLocator& locator, std::string window_name);
        static void show_internals(DeclarativeBroccoliLocator& locator);
};
