CMAKE_MINIMUM_REQUIRED(VERSION 2.8.0 FATAL_ERROR)

# Project
PROJECT(BroccoliBot)

# Project commands are put in compile_commands.json to be usedwith IDEs
SET( CMAKE_EXPORT_COMPILE_COMMANDS 1 )

FIND_PACKAGE(OpenCV)
FIND_PACKAGE(realsense2)

# Compile source files
FILE(GLOB SOURCES "src/*.cpp")
INCLUDE_DIRECTORIES("./include")

# Use c++11 and clang
SET (CMAKE_CXX_COMPILER "/usr/bin/clang++")
SET (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17 ")

# Make all of the sources executable
ADD_EXECUTABLE(BroccoliBot ${SOURCES})

# Link libraries into the executable
TARGET_LINK_LIBRARIES(BroccoliBot ${OpenCV_LIBS} ${realsense2_LIBRARY})
