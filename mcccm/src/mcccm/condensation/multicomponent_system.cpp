//
// Created by egor on 12/15/24.
//

#include <mcccm/condensation/multicomponent_system.h>
#include <mcccm/geometry/geometry.h>

// Unit tests below, compiled only is this is a test target
// available only in a native build
#ifdef DO_TEST

#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_floating_point.hpp>

TEST_CASE("MulticomponentSystem with ConstantMeanCurvatureSurface geometry interface tested",
    "[MulticomponentSystem<ConstantMeanCurvatureSurface>]") {
    // Geometry interface
    double r_part = 14.0e-9;
    double neck_fa = 10.0;
    double ca = 0.0;
    auto surface_tension = [](std::vector<double> const &) -> double {
        return 0.05;
    };
    auto temperature = [](double) -> double {
        return 300.0;
    };
    auto saturation = [](double, unsigned long) -> double {
        return 1.0;
    };
    std::vector<Component> components {
        components::MOSAIC::API1, components::MOSAIC::API2
    };

    geometry_interfaces::ConstantMeanCurvatureSurface geom_interface(ca, neck_fa, r_part);
    MulticomponentSystem system(geom_interface, temperature, saturation, surface_tension, components);
    MulticomponentSystem<geometry_interfaces::ConstantMeanCurvatureSurface>::StateBuffer v(components.size()), dvdt(components.size());

    // Test equilibrium mole fractions
    auto equilibrium_mole_fractions = system.get_equilibrium_mole_fractions(temperature(0.0));
    REQUIRE(equilibrium_mole_fractions[0]
        == components[0].get_p_sat(temperature(0.0)) / (components[0].get_p_sat(temperature(0.0)) + components[1].get_p_sat(temperature(0.0))));
    REQUIRE(equilibrium_mole_fractions[1]
        == components[1].get_p_sat(temperature(0.0)) / (components[0].get_p_sat(temperature(0.0)) + components[1].get_p_sat(temperature(0.0))));

    // Test that mole fractions add up to one
    REQUIRE_THAT(equilibrium_mole_fractions[0] + equilibrium_mole_fractions[1], Catch::Matchers::WithinAbs(1.0, 0.000001));

    // Test average molar volume
    double average_molar_volume = system.get_average_molar_volume(equilibrium_mole_fractions);
    REQUIRE(equilibrium_mole_fractions[0] * components[0].get_molar_volume()
        + equilibrium_mole_fractions[1] * components[1].get_molar_volume()
        == average_molar_volume);

    // No condensate
    v[0] = 0.0;
    v[1] = 0.0;
    system(v, dvdt, 0.0);
    REQUIRE(dvdt[0] > 0.0);
    REQUIRE(dvdt[0] > 0.0);

    // Max condensate volume
    double vol_cylinder = M_PI * r_part * r_part * 2.0 * r_part - 4.0 / 3.0 * M_PI * r_part * r_part * r_part;
    double max_vol_condensate = vol_cylinder - geom_interface.get_neck_volume();
    v[0] = max_vol_condensate;
    v[1] = 0.0;
    system(v, dvdt, 0.0);
    REQUIRE(dvdt[0] < 0.0);

    v[0] = 0.0;
    v[1] = max_vol_condensate;
    system(v, dvdt, 0.0);
    REQUIRE(dvdt[1] < 0.0);
}

#endif //DO_TEST

