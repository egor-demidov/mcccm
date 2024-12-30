cmake_minimum_required(VERSION 3.28)
project(mcccm VERSION 0.0.1)

find_package(fmt CONFIG REQUIRED)
find_package(Boost REQUIRED COMPONENTS numeric_odeint)
find_package(Catch2 CONFIG REQUIRED)

add_compile_definitions(_USE_MATH_DEFINES)

# Set environment variables
include(${PROJECT_SOURCE_DIR}/cmake/SetEnv.cmake)

add_subdirectory(mcccm)
