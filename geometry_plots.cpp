//
// Created by egor on 12/15/24.
//

#include <fstream>

#include <fmt/ostream.h>

#include "geometry/geometry.h"

int main() {
    std::ofstream ofs("geometry.csv");

    if (!ofs.good()) {
        throw std::exception();
    }

    GeometryInterpolator interp(0.0);

    unsigned long n_fa = Geometry_t::n_fa * 2ul;
    double dfa = (generated_geometry.end_fa - generated_geometry.begin_fa) / static_cast<double>(n_fa - 1);
    fmt::println(ofs, "fa, volume, area, kappa");
    for (unsigned long i = 0; i < n_fa; i ++) {
        double fa = generated_geometry.begin_fa + dfa * static_cast<double>(i);
        double volume = interp.interpolate_volume(fa);
        double area = interp.interpolate_area(fa);
        double kappa = interp.interpolate_kappa(fa);
        fmt::println(ofs, "{}, {}, {}, {}", fa, volume, area, kappa);
    }

    return 0;
}