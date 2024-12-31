# Multicomponent Capillary Condensation Model (MCCCM)

## Building library from source

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
