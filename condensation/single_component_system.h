//
// Created by egor on 12/15/24.
//

#ifndef SINGLE_COMPONENT_SYSTEM_H
#define SINGLE_COMPONENT_SYSTEM_H

#include <cmath>

#include "component.h"
#include "../physical_constants.h"

template <typename GeometryInterface>
class SingleComponentSystem {
public:
    using StateBuffer = std::vector<double>;

    SingleComponentSystem(
        GeometryInterface geometry_interface,
        std::function<double(double)> temperature,
        std::function<double(double)> saturation,
        double surface_tension,
        Component component
    )
        : geometry_interface{std::move(geometry_interface)}
        , temperature{std::move(temperature)}
        , saturation{std::move(saturation)}
        , surface_tension{surface_tension}
        , component{std::move(component)}
    {}

    double thermal_velocity(double temp) const {
        return sqrt(8.0 * gas_constant * temp / M_PI / component.get_molecular_weight());
    }

    double kelvin_length(double temp) const {
        return 2.0 * surface_tension * component.get_molar_volume() / gas_constant / temp;
    }

    // ODE function to be used with the integrator
    void operator()(StateBuffer const & v, StateBuffer & dvdt, const double t) const {

        double v_tot = v[0];

        if (v_tot > geometry_interface.get_max_liquid_volume())
            v_tot = geometry_interface.get_max_liquid_volume();

        double temp = temperature(t);
        auto [area, kappa] = geometry_interface.get_liquid_props(v_tot);
        dvdt[0] = 1.0 / 4.0 * area * thermal_velocity(temp) * component.get_molar_volume()
            * component.get_p_sat(temp) / gas_constant / temp
            * (saturation(t) - exp(0.5 * kelvin_length(temp) * kappa));
        if (v_tot <= 0.0 && dvdt[0] < 0.0)
            dvdt[0] = 0.0;
    }

    unsigned long get_num_components() const {
        return n_components;
    }

private:
    const GeometryInterface geometry_interface;
    const std::function<double(double)> temperature, saturation;
    const double surface_tension;
    Component component;

    static constexpr unsigned long n_components = 1ul;
};

#endif //SINGLE_COMPONENT_SYSTEM_H
