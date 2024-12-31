# Multicomponent Capillary Condensation Model (MCCCM)

## Building the library from source

### Linux / macOS

Download the latest source code from the Git repository:
```shell
git clone https://github.com/egor-demidov/mcccm.git
```
Change into the source directory and configure with CMake, vcpkg, and Ninja:
```shell
cd mcccm && cmake --preset="release-gnu" . 
```
Build the project:
```shell
cmake --build build-release-gnu
```
Install the library in a location of choice:
```shell
cmake --install build-release-gnu --prefix install-release-gnu
```

### Windows (MSVC compiler)

> [!WARNING]  
> The following commands need to be run in an instance of the "Developer Power Shell for Visual Studio"

Download the latest source code from the Git repository:
```shell
git clone https://github.com/egor-demidov/mcccm.git
```
Change into the source directory and configure with CMake, vcpkg, and Ninja:
```shell
cd mcccm; cmake --preset="release-win" . 
```
Build the project:
```shell
cmake --build build-release-win
```
Install the library in a location of choice:
```shell
cmake --install build-release-win --prefix install-release-win
```

## Using the library in a CMake-based project

When configuring your CMake-based project, specify the location where MCCCM was installed
through
the `mcccm_DIR` CMake cache variable:
```shell
cmake --Dmcccm_DIR=<mcccm_install_dir>/lib/cmake/mcccm <remaining_config_command>
```
In your `CMakeLists.txt` file, find and link MCCCM:
```cmake
find_package(mcccm REQUIRED)

# Target creation, configuration, etc.

target_link_libraries(<your_target_name> PRIVATE mcccm::mcccm)
```

## Single component condensation example
Include the header:
```c++
#include <mcccm/condensation/condensation_implementations.h>
```
A number of components are pre-defined in the `components::` and `components::MOSAIC::` namespaces.
Select one of those or define a custom component. To define a custom component, you need a function that computes
the saturated vapor pressure (in Pascals) of the component as a function of temperature. This can be implemented as a lambda function:
```c++
auto p_sat_custom_component = [](double temperature) -> double {
    return 1.0e5 * pow(10.0, 6.07936 - 2692.187 / (temperature - 17.94));
};
```
The define you custom component as:
```c++
Component PG {
    "Propylene glycol",     // Component name
    1040.0,                 // Component density, kg/m^3
    0.076,                  // Component molar weight, kg/mol
    p_sat_custom_component  // Saturated vapor presure function, Pa(K) 
};
```
To invoke single component condensation, a more parameters need to be defined.
Saturation and temperature (in Kelvins) as functions of time (in seconds)
need to be provided:
```c++
auto temperature = [](double time [[maybe_unused]]) -> double {
    return 300.0;
};

auto saturation = [](double time [[maybe_unusec]]) -> double {
    return 1.0;
};
```
Then, particle radius (in meters), neck filling angle (in degrees),
coating contact angle (in degrees), surface tension (in Newtons per meter),
integration time step (in seconds), and total simulation time (in seconds)
need to be defined as constants:
```c++
double r_part = 14.0e-9;
double neck_fa = 10.0;
double ca = 0.0;
double surface_tension = 0.05;
double dt =  0.001;
double t_tot = 1.0;
```
Finally, singe component condensation can be executed with:
```c++
SingleComponentCombinedCondensationRun cond(temperature, saturation, surface_tension, PG, r_part, ca, neck_fa, t_tot, dt);
```
The number of points in the solution can be accessed with:
```c++
cond.get_n_points();
```
And the results array can be accessed with:
```c++
cond.get_capillary_condensation_results();
```
Each element of the results array is an instance of the following structure:
```c++
struct Solution {
    double time;
    double condensate_volume;
    double condensate_volume_fraction;
    double uniform_to_capillary_ratio;
    double capillary_filling_angle;
    double uniform_coating_thickness;
    double capillary_condensate_volume;
    double uniform_condensate_volume;
};
```
