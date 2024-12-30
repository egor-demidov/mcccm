//
// Created by egor on 12/14/24.
//

#ifndef MULTICOMPONENT_SYSTEM_H
#define MULTICOMPONENT_SYSTEM_H

#include <cmath>

#include "component.h"
#include "../physical_constants.h"
#include "../geometry/geometry.h"

template <typename GeometryInterface>
class MulticomponentSystem {
public:
    using StateBuffer = std::vector<double>;

    MulticomponentSystem(
        GeometryInterface geometry_interface,
        std::function<double(double)> temperature,
        std::function<double(double, unsigned long)> saturation,
        std::function<double(StateBuffer const &)> surface_tension,
        std::vector<Component> components
    )
        : geometry_interface{std::move(geometry_interface)}
        , temperature{std::move(temperature)}
        , saturation{std::move(saturation)}
        , surface_tension{std::move(surface_tension)}
        , components{std::move(components)}
        , n_components{this->components.size()}
    {}

    unsigned long get_num_components() const {
        return n_components;
    }

    double get_average_molar_volume(StateBuffer const & mole_fractions) const {
        double average_moler_volume = 0.0;
        for (unsigned long i = 0; i < get_num_components(); i ++) {
            average_moler_volume += mole_fractions[i] * components[i].get_molar_volume();
        }
        return average_moler_volume;
    }

    double thermal_velocity(double temperature, unsigned long i) const {
        return sqrt(8.0 * gas_constant * temperature / M_PI / components[i].get_molecular_weight());
    }

    double kelvin_length(double temp, StateBuffer const & mole_fractions) const {
        return 2.0 * surface_tension(mole_fractions) * get_average_molar_volume(mole_fractions) / gas_constant / temp;
    }

    StateBuffer get_equilibrium_mole_fractions(double temp) const {

        StateBuffer mole_fractions(get_num_components());

        for (unsigned long i = 0; i < get_num_components(); i ++) {
            mole_fractions[i] = components[i].get_p_sat(temp);
        }

        double p_sat_sum = std::accumulate(mole_fractions.begin(), mole_fractions.end(), 0.0);

        for (double & mole_frac_i : mole_fractions) {
            mole_frac_i /= p_sat_sum;
        }

        return mole_fractions;
    }

    // ODE function to be used with the integrator
    void operator()(const StateBuffer & v, StateBuffer & dvdt, const double t) const {

        double v_tot = std::accumulate(v.begin(), v.end(), 0.0);

        if (v_tot > geometry_interface.get_max_liquid_volume())
            v_tot = geometry_interface.get_max_liquid_volume();

        double temp = temperature(t);
        auto [area, kappa] = geometry_interface.get_liquid_props(v_tot);

        StateBuffer mole_fractions(get_num_components());
        if (v_tot <= 0.0)
            mole_fractions = get_equilibrium_mole_fractions(temp);
        else {
            for (unsigned long i = 0; i < get_num_components(); i ++)
                mole_fractions[i] = v[i] / components[i].get_molar_volume();

            double total_mols = std::accumulate(mole_fractions.begin(), mole_fractions.end(), 0.0);

            for (double & mol_frac : mole_fractions)
                mol_frac /= total_mols;
        }

        double l_k = kelvin_length(temp, mole_fractions);
        for (unsigned long i = 0; i < get_num_components(); i ++)
            dvdt[i] = 1.0 / 4.0 * area * thermal_velocity(temp, i) * components[i].get_molar_volume()
                * components[i].get_p_sat(temp) / gas_constant / temp
                * (saturation(t, i) - mole_fractions[i] * exp(0.5 * l_k * kappa));

        if (v_tot <= 0.0)
            for (unsigned long i = 0; i < get_num_components(); i ++)
                if (v_tot <= 0.0 && dvdt[i] < 0.0)
                    dvdt[i] = 0.0;

        if (v_tot >= geometry_interface.get_max_liquid_volume())
            for (unsigned long i = 0; i < get_num_components(); i ++)
                if (dvdt[i] > 0.0)
                    dvdt[i] = 0.0;
    }

private:
    const GeometryInterface geometry_interface;
    const std::function<double(double)> temperature;
    const std::function<double(double, unsigned long)> saturation;
    const std::function<double(StateBuffer const &)> surface_tension;
    const std::vector<Component> components;

    const unsigned long n_components;
};

#endif //MULTICOMPONENT_SYSTEM_H
