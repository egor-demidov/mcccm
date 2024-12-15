//
// Created by egor on 12/15/24.
//

#include <fmt/format.h>

#include "run_condensation.h"
#include "component.h"
#include "../geometry/geometry.h"
#include "single_component_system.h"

// Unit tests below, compiled only is this is a test target
// available only in a native build
#ifdef DO_TEST

#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_floating_point.hpp>

TEST_CASE("Single component condensation tested",
    "[CondensationRun<SingleComponentSystem<ConstantMeanCurvatureSurface>>]") {
    // Geometry interface
    double r_part = 14.0e-9;
    double neck_fa = 10.0;
    double ca = 0.0;
    double surface_tension = 0.05;
    auto temperature = [](double) -> double {
        return 300.0;
    };
    auto saturation = [](double) -> double {
        return 1.0;
    };
    double t_tot = 10.0;

    geometry_interfaces::ConstantMeanCurvatureSurface geom_interface(ca, neck_fa, r_part);
    SingleComponentSystem system(geom_interface, temperature, saturation, surface_tension, components::MOSAIC::API1);
    CondensationRun run(system, {0.0}, t_tot);

    fmt::println("t\tv\tfa");
    for (unsigned long i = 0; i < run.get_n_steps(); i ++) {
        fmt::println("{}\t{}\t{}", run.get_time()[i], run.get_solution()[i][0], geom_interface.get_filling_angle(run.get_solution()[i][0]));
    }
}

#endif //DO_TEST
