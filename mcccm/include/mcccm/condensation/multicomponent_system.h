//
// Created by egor on 12/14/24.
//

#ifndef MULTICOMPONENT_SYSTEM_H
#define MULTICOMPONENT_SYSTEM_H

#include <cmath>

#include "component.h"
#include "../physical_constants.h"
#include "../geometry/geometry.h"

class MulticomponentSystem {
public:
    using StateBuffer = std::vector<double>;

    MulticomponentSystem(
        double neck_filling_angle,
        double contact_angle,
        double particle_radius,
        double residence_time,
        std::function<double(double)> temperature,
        std::function<double(double)> saturation,
        std::function<double(std::vector<double> const &)> surface_tension,
        std::vector<Component> components
    );

    unsigned long get_num_components() const {
        return num_components;
    }

    double thermal_velocity(double temperature) const {
        return sqrt(8.0 * gas_constant * temperature / M_PI / components[0].get_molecular_weight());
    }

    // ODE function to be used with the integrator
    void operator()(const StateBuffer & v, StateBuffer & dvdt, const double t) const {
        double temp = temperature(t);
        double l_k = sqrt(2.0 * surface_tension(std::vector<double>()) * components[0].get_molar_volume() / gas_constant / temp);
        double liquid_volume = v[0];
        double meniscus_volume = liquid_volume + neck_volume;
        auto [filling_angle, fa_interpolator] = liquid_geometry_interpolator.volume_to_filling_angle(meniscus_volume);
        double kappa = liquid_geometry_interpolator.interpolate_kappa(fa_interpolator);
        double area = liquid_geometry_interpolator.interpolate_area(fa_interpolator);
        dvdt[0] = 1.0 / 4.0 * area * thermal_velocity(temp) * components[0].get_molar_volume() * components[0].get_p_sat(temp) / avogadro_constant / gas_constant / temp * (saturation(t) - exp(0.5 * l_k * kappa));
        if (meniscus_volume <= 0.0 && dvdt[0] < 0.0)
            dvdt[0] = 0.0;
    }

private:
    const double neck_filling_angle, contact_angle, particle_radius, residence_time;
    const std::function<double(double)> temperature, saturation;
    const std::function<double(std::vector<double> const &)> surface_tension;
    const std::vector<Component> components;
    const GeometryInterpolator neck_geometry_interpolator, liquid_geometry_interpolator;
    const unsigned long num_components;
    const double neck_volume;
};

#endif //MULTICOMPONENT_SYSTEM_H
