//
// Created by egor on 12/22/24.
//

#ifndef CONDENSATION_IMPLEMENTATIONS_H
#define CONDENSATION_IMPLEMENTATIONS_H

#include "component.h"
#include "run_condensation.h"
#include "single_component_system.h"
#include "../geometry/geometry.h"

class SingleComponentCapillaryCondensationRun {
public:

    struct Result {
        double time;
        double condensate_volume;
        double condensate_volume_fraction;
        double uniform_to_capillary_ratio;
        double capillary_filling_angle;
        double uniform_coating_thickness;
    };

    SingleComponentCapillaryCondensationRun(
        std::function<double(double)> const & temperature,
        std::function<double(double)> const & saturation,
        double surface_tension,
        Component const & component,
        double r_part,
        double contact_angle,
        double neck_filling_angle,
        double t_tot,
        double dt,
        unsigned long dump_period = 1ul
    );

    std::vector<Result> const & get_capillary_condensation_results() const;
    unsigned long get_n_points() const;

private:
    // Geometry interfaces
    const geometry_interfaces::SphericalSurface spherical_surface;
    const geometry_interfaces::ConstantMeanCurvatureSurface constant_mean_curvature_surface;

    // Single component systems
    const SingleComponentSystem<geometry_interfaces::SphericalSurface> spherical_single_component_system;
    const SingleComponentSystem<geometry_interfaces::ConstantMeanCurvatureSurface> constant_mean_curvature_single_component_system;

    // Condensation runs with solution buffers
    const CondensationRun<SingleComponentSystem<geometry_interfaces::SphericalSurface>> spherical_condensation_run;
    const CondensationRun<SingleComponentSystem<geometry_interfaces::ConstantMeanCurvatureSurface>> constant_mean_curvature_condensation_run;

    // Post-processed results
    std::vector<Result> condensation_results;
    unsigned long n_points;
};

#endif //CONDENSATION_IMPLEMENTATIONS_H
