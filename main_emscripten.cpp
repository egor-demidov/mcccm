#include <vector>
#include <numeric>

#include <fmt/format.h>
#include <emscripten/bind.h>

#include "condensation/condensation_implementations.h"
#include "git.h"

double thermal_velocity(double temp, Component const & component) {
    return sqrt(8.0 * gas_constant * temp / M_PI / component.get_molecular_weight());
}

double estimate_characteristic_condensation_rate(
        Component const & component,
        double r_part,
        double temperature,
        double saturation
    ) {

    // Characteristic area - surface of a monomer
    const double area = 4.0 * M_PI * r_part * r_part * r_part;
    return 1.0 / 4.0 * area * thermal_velocity(temperature, component) * component.get_molar_volume()
            * component.get_p_sat(temperature) / gas_constant / temperature * saturation;
}

static constexpr double TIMESTEP_FACTOR = 1.0e-32;
static constexpr unsigned long N_POINTS =  50;

class LargeTimestepException : public std::exception {
public:
    const char * what() const noexcept override {
        return "Timestep too large - increase the simulation time";
    }
};

struct CapillaryCondensationResult {
    std::vector<SingleComponentCapillaryCondensationRun::Solution> solution;
    unsigned long n_steps;
    double dt;
};

std::string get_condensation_engine_tag() {
    return std::string(git::CommitSHA1()) + " (" + std::string(git::Branch()) + ")";
}

CapillaryCondensationResult run_single_component_capillary_condensation(
    Component const & component,
    double r_part,
    double neck_fa,
    double ca,
    double surface_tension,
    double temperature,
    double saturation,
    double t_tot
) {

    auto temperature_fun = [temperature](double) -> double {
        return temperature;
    };
    auto saturation_fun = [saturation](double) -> double {
        return saturation;
    };

    // Timestep inversely proportional to the characteristic condensation rate
    const double dt = TIMESTEP_FACTOR / estimate_characteristic_condensation_rate(component, r_part, temperature, saturation);

    if (2.0 * dt >= t_tot)
        throw LargeTimestepException();

    const unsigned long n_steps = static_cast<unsigned long>(t_tot / dt);
    const unsigned long dump_period = n_steps / (N_POINTS - 1);

    SingleComponentCapillaryCondensationRun cond(temperature_fun, saturation_fun, surface_tension, component, r_part, ca, neck_fa, t_tot, dt, dump_period);

    return {
        .solution = cond.get_capillary_condensation_results(),
        .n_steps = n_steps,
        .dt = dt
    };
}

EMSCRIPTEN_BINDINGS(capillary_condensation) {
    emscripten::function("run_single_component_capillary_condensation", &run_single_component_capillary_condensation);
    emscripten::function("get_condensation_engine_tag", &get_condensation_engine_tag);

    emscripten::register_vector<SingleComponentCapillaryCondensationRun::Solution>("VectorResult");

    emscripten::class_<Component>("Component")
        .constructor<std::string, double, double, double>()
        .property("name_readonly", &Component::get_name)
        .property("density_readonly", &Component::get_density)
        .property("molecular_weight_readonly", &Component::get_molecular_weight)
        .function("p_sat", &Component::get_p_sat)
        .property("molar_volume_readonly", &Component::get_molar_volume);

    emscripten::value_object<SingleComponentCapillaryCondensationRun::Solution>("CondensationSolution")
        .field("time", &SingleComponentCapillaryCondensationRun::Solution::time)
        .field("condensate_volume", &SingleComponentCapillaryCondensationRun::Solution::condensate_volume)
        .field("condensate_volume_fraction", &SingleComponentCapillaryCondensationRun::Solution::condensate_volume_fraction)
        .field("uniform_to_capillary_ratio", &SingleComponentCapillaryCondensationRun::Solution::uniform_to_capillary_ratio)
        .field("capillary_filling_angle", &SingleComponentCapillaryCondensationRun::Solution::capillary_filling_angle)
        .field("uniform_coating_thickness", &SingleComponentCapillaryCondensationRun::Solution::uniform_coating_thickness)
        .field("capillary_condensate_volume", &SingleComponentCapillaryCondensationRun::Solution::capillary_condensate_volume)
        .field("uniform_condensate_volume", &SingleComponentCapillaryCondensationRun::Solution::uniform_condensate_volume);

    emscripten::value_object<CapillaryCondensationResult>("CapillaryCondensationResult")
        .field("solution", &CapillaryCondensationResult::solution)
        .field("n_steps", &CapillaryCondensationResult::n_steps)
        .field("dt", &CapillaryCondensationResult::dt);
}
