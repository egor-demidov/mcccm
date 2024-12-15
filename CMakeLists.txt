cmake_minimum_required(VERSION 3.28)
project(wasm_capillary_condensation)

set(CMAKE_CXX_STANDARD 20)

find_package(fmt CONFIG REQUIRED)
find_package(Boost REQUIRED COMPONENTS numeric_odeint)

option(EMSCRIPTEN "Target emscripten" OFF)

add_compile_definitions(_USE_MATH_DEFINES)

if (${EMSCRIPTEN})
    set(CMAKE_CXX_FLAGS "-s EXPORT_ES6=1 -s MODULARIZE=1 ${CMAKE_CXX_FLAGS}")
    set(EMSCRIPTEN_LINK_FLAGS "--bind")
else ()
    # Native executables should come with a test executable
    find_package(Catch2 CONFIG REQUIRED)
endif ()

set(AUX_SOURCES geometry/geometry.h
        geometry/generated_geometry.cpp
        geometry/geometry.cpp
        condensation/component.cpp
        condensation/component.h
        physical_constants.h
        condensation/multicomponent_system.cpp
        condensation/multicomponent_system.h
        condensation/single_component_system.cpp
        condensation/single_component_system.h
        condensation/run_condensation.cpp
        condensation/run_condensation.h
)

if (${EMSCRIPTEN})
    add_executable(wasm_capillary_condensation main_emscripten.cpp ${AUX_SOURCES})
else ()
    add_executable(wasm_capillary_condensation main.cpp ${AUX_SOURCES})

    add_executable(geometry_plots geometry_plots.cpp ${AUX_SOURCES})
    target_link_libraries(geometry_plots PRIVATE fmt::fmt Boost::numeric_odeint)

    add_executable(unit_tests ${AUX_SOURCES})
    target_compile_definitions(unit_tests PUBLIC DO_TEST=1)
    target_link_libraries(unit_tests PRIVATE Catch2::Catch2WithMain fmt::fmt Boost::numeric_odeint)
endif ()

target_link_libraries(wasm_capillary_condensation PRIVATE ${EMSCRIPTEN_LINK_FLAGS} fmt::fmt Boost::numeric_odeint)
