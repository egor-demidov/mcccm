cmake_minimum_required(VERSION 3.28)
project(wasm_capillary_condensation)

set(CMAKE_CXX_STANDARD 17)

find_package(fmt CONFIG REQUIRED)
find_package(Boost REQUIRED COMPONENTS numeric_odeint)

set(CMAKE_CXX_FLAGS "-s EXPORT_ES6=1 -s MODULARIZE=1 ${CMAKE_CXX_FLAGS}")

add_executable(wasm_capillary_condensation main.cpp geometry/geometry.h)

target_link_libraries(wasm_capillary_condensation PRIVATE --bind fmt::fmt Boost::numeric_odeint)
