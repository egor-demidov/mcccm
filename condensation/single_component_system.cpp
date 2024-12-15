//
// Created by egor on 12/15/24.
//

#include "single_component_system.h"
#include "../geometry/geometry.h"

// Unit tests below, compiled only is this is a test target
// available only in a native build
#ifdef DO_TEST

#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_floating_point.hpp>

TEST_CASE("SingleComponentSystem with ConstantMeanCurvatureSurface geometry interface tested",
    "[SingleComponentSystem<ConstantMeanCurvatureSurface>]") {
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

    geometry_interfaces::ConstantMeanCurvatureSurface geom_interface(ca, neck_fa, r_part);
    SingleComponentSystem system(geom_interface, temperature, saturation, surface_tension, components::MOSAIC::API1);
    SingleComponentSystem<geometry_interfaces::ConstantMeanCurvatureSurface>::StateBuffer v(1), dvdt(1);

    // No condensate
    v[0] = 0.0;
    system(v, dvdt, 0.0);
    REQUIRE(dvdt[0] > 0.0);

    // Max condensate volume
    double vol_cylinder = M_PI * r_part * r_part * 2.0 * r_part - 4.0 / 3.0 * M_PI * r_part * r_part * r_part;
    double max_vol_condensate = vol_cylinder - geom_interface.get_neck_volume();
    v[0] = max_vol_condensate;
    system(v, dvdt, 0.0);
    REQUIRE(dvdt[0] < 0.0);
}

#endif //DO_TEST
