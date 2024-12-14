//
// Created by Egor Demidov on 12/12/24.
//

#ifndef WASM_CAPILLARY_CONDENSATION_GEOMETRY_H
#define WASM_CAPILLARY_CONDENSATION_GEOMETRY_H

#include <array>

template <unsigned long num_fa, unsigned long num_ca>
struct Geometry {

    struct CASeries {
        const double ca;
        const std::array<double, num_fa> filling_angle, volume, area, kappa;
    };

    const double begin_fa, end_fa;
    const double begin_ca, end_ca;
    const std::array<CASeries, num_ca> ca_sets;

    static constexpr unsigned long n_fa = num_fa;
    static constexpr unsigned long n_ca = num_ca;
};

#include "generated_geometry.h"

#endif //WASM_CAPILLARY_CONDENSATION_GEOMETRY_H
