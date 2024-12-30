set(MCCCM_SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src/mcccm)
set(MCCCM_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include)

set(MCCCM_SRC
        ${MCCCM_SRC_DIR}/geometry/generated_geometry.cpp
        ${MCCCM_SRC_DIR}/geometry/geometry.cpp
        ${MCCCM_SRC_DIR}/condensation/component.cpp
        ${MCCCM_SRC_DIR}/condensation/multicomponent_system.cpp
        ${MCCCM_SRC_DIR}/condensation/single_component_system.cpp
        ${MCCCM_SRC_DIR}/condensation/run_condensation.cpp
        ${MCCCM_SRC_DIR}/condensation/condensation_implementations.cpp
)

set(MCCCM_INCLUDE
        ${MCCCM_INCLUDE_DIR}/mcccm/condensation/component.h
        ${MCCCM_INCLUDE_DIR}/mcccm/condensation/condensation_implementations.h
        ${MCCCM_INCLUDE_DIR}/mcccm/condensation/multicomponent_system.h
        ${MCCCM_INCLUDE_DIR}/mcccm/condensation/run_condensation.h
        ${MCCCM_INCLUDE_DIR}/mcccm/condensation/single_component_system.h
        ${MCCCM_INCLUDE_DIR}/mcccm/geometry/generated_geometry.h
        ${MCCCM_INCLUDE_DIR}/mcccm/geometry/geometry.h
        ${MCCCM_INCLUDE_DIR}/mcccm/physical_constants.h
)

set(HEADER_DIR ${MCCCM_INCLUDE_DIR})

# Set SOURCES variable
set(SOURCES ${MCCCM_SRC})

# Set HEADERS_PUBLIC variable (public headers, included in the library)
set(HEADERS_PUBLIC ${MCCCM_INCLUDE})

include(${PROJECT_SOURCE_DIR}/cmake/LibraryConfig.cmake)

# Generate an executable with unit tests
add_executable(mcccm_unit_tests ${MCCCM_SRC} ${MCCCM_INCLUDE})
target_include_directories(mcccm_unit_tests PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<BUILD_INTERFACE:${CMAKE_BINARY_DIR}>
        $<INSTALL_INTERFACE:include>
)
target_compile_features(mcccm_unit_tests PUBLIC cxx_std_20)
target_compile_definitions(mcccm_unit_tests PUBLIC DO_TEST=1)
target_link_libraries(mcccm_unit_tests PRIVATE Catch2::Catch2WithMain fmt::fmt Boost::numeric_odeint)
