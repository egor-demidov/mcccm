//
// Created by egor on 12/15/24.
//

#include <fmt/format.h>

#include <mcccm/condensation/run_condensation.h>
#include <mcccm/condensation/component.h>
#include <mcccm/geometry/geometry.h>
#include <mcccm/condensation/single_component_system.h>

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
    double t_tot = 1.0;

    geometry_interfaces::ConstantMeanCurvatureSurface geom_interface(ca, neck_fa, r_part);
    SingleComponentSystem system(geom_interface, temperature, saturation, surface_tension, components::TEG);
    CondensationRun run(system, {0.0}, t_tot, 0.01);

    double target_fa = 55.64623269487302;
    REQUIRE_THAT(target_fa, Catch::Matchers::WithinAbs(geom_interface.get_filling_angle(run.get_solution().back()[0]), 0.000001));
}

TEST_CASE("Single component condensation tested",
    "[CondensationRun<SingleComponentSystem<SphericalSurface>>]") {
    // Geometry interface
    double r_part = 100.0e-9;
    double surface_tension = 0.05;
    auto temperature = [](double) -> double {
        return 300.0;
    };
    auto saturation = [](double) -> double {
        return 1.5;
    };
    double t_tot = 1.0;

    geometry_interfaces::SphericalSurface geom_interface(r_part);
    SingleComponentSystem system(geom_interface, temperature, saturation, surface_tension, components::TEG);
    CondensationRun run(system, {0.0}, t_tot, 0.01);

    double target_r_equiv = 2.281894653033472e-07;
    REQUIRE_THAT(target_r_equiv, Catch::Matchers::WithinAbs(geom_interface.get_equivalent_radius(run.get_solution().back()[0]), 0.000001));
}

#endif //DO_TEST
