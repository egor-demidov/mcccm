//
// Created by egor on 12/22/24.
//

#include "condensation_implementations.h"

SingleComponentCapillaryCondensationRun::SingleComponentCapillaryCondensationRun(
    std::function<double(double)> const & temperature,
    std::function<double(double)> const & saturation,
    double surface_tension,
    Component const & component,
    double r_part,
    double contact_angle,
    double neck_filling_angle,
    double t_tot,
    double dt,
    unsigned long dump_period
)
    : spherical_surface{r_part}
    , constant_mean_curvature_surface{contact_angle, neck_filling_angle, r_part}
    , spherical_single_component_system{spherical_surface, temperature, saturation, surface_tension, component}
    , constant_mean_curvature_single_component_system{constant_mean_curvature_surface, temperature, saturation, surface_tension, component}
    , spherical_condensation_run{spherical_single_component_system, {0.0}, t_tot, dt, dump_period}
    , constant_mean_curvature_condensation_run{constant_mean_curvature_single_component_system, {0.0}, t_tot, dt, dump_period}
{
    condensation_results.resize(spherical_condensation_run.get_n_points());

    for (unsigned long n = 0; n < spherical_condensation_run.get_n_points(); n ++) {
        const double time = spherical_condensation_run.get_time()[n];
        const double capillary_condensate_volume = constant_mean_curvature_condensation_run.get_solution()[n][0];
        const double capillary_filling_angle = constant_mean_curvature_surface.get_filling_angle(capillary_condensate_volume);
        const double spherical_condensate_volume = spherical_condensation_run.get_solution()[n][0];
        const double spherical_equivalent_radius = spherical_surface.get_equivalent_radius(spherical_condensate_volume);
        const double uniform_coating_thickness = spherical_equivalent_radius - r_part;
        const double bare_sector_volume = 2.0 / 3.0 * M_PI * r_part * r_part * r_part * (1.0 - cos(capillary_filling_angle * M_PI / 180.0));
        const double coated_sector_volume = 2.0 / 3.0 * M_PI * uniform_coating_thickness * uniform_coating_thickness * uniform_coating_thickness * (1.0 - cos(capillary_filling_angle * M_PI / 180.0));
        const double effective_uniform_coating_volume = spherical_condensate_volume - 2.0 * (coated_sector_volume - bare_sector_volume);
        const double uniform_to_capillary_ratio = effective_uniform_coating_volume / capillary_condensate_volume;
        const double core_volume = spherical_surface.get_core_volume() + constant_mean_curvature_surface.get_neck_volume();
        const double total_condensate_volume = effective_uniform_coating_volume + capillary_condensate_volume;
        const double condensate_volume_fraction = total_condensate_volume / (core_volume + total_condensate_volume);

        condensation_results[n] = {
            time,
            total_condensate_volume,
            condensate_volume_fraction,
            uniform_to_capillary_ratio,
            capillary_filling_angle,
            uniform_coating_thickness,
            static_cast<unsigned long>(t_tot / dt),
            dt
        };
    }
}

std::vector<SingleComponentCapillaryCondensationRun::Result> const & SingleComponentCapillaryCondensationRun::get_capillary_condensation_results() const {
    return condensation_results;
}

unsigned long SingleComponentCapillaryCondensationRun::get_n_points() const {
    return n_points;
}


// Unit tests below, compiled only is this is a test target
// available only in a native build
#ifdef DO_TEST

#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_floating_point.hpp>

#include <fmt/format.h>

TEST_CASE("Testing SingleComponentCapillaryCondensationRun", "[SingleComponentCapillaryCondensationRun]") {
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

    SingleComponentCapillaryCondensationRun cond(temperature, saturation, surface_tension, components::TEG, r_part, ca, neck_fa, t_tot, 0.001);

    // There should be no uniform condensate
    REQUIRE_THAT(0.0, Catch::Matchers::WithinAbs(cond.get_capillary_condensation_results()[cond.get_n_points() - 1].uniform_coating_thickness, 0.000001));
    REQUIRE_THAT(0.0, Catch::Matchers::WithinAbs(cond.get_capillary_condensation_results()[cond.get_n_points() - 1].uniform_to_capillary_ratio, 0.000001));
}

#endif //DO_TEST
