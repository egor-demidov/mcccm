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

## Using the library in a CMake project

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
