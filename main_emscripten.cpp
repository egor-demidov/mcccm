#include <vector>
#include <numeric>

#include <fmt/format.h>
#include <emscripten/bind.h>

#include "condensation/condensation_implementations.h"
#include "git.h"

std::vector<SingleComponentCapillaryCondensationRun::Result> run_single_component_capillary_condensation(
        Component const & component,
        double r_part,
        double neck_fa,
        double ca,
        double surface_tension,
        double temperature,
        double saturation,
        double dt,
        double t_tot
    ) {

    auto temperature_fun = [temperature](double) -> double {
        return temperature;
    };
    auto saturation_fun = [saturation](double) -> double {
        return saturation;
    };

    SingleComponentCapillaryCondensationRun cond(temperature_fun, saturation_fun, surface_tension, component, r_part, ca, neck_fa, t_tot, dt);
    return cond.get_capillary_condensation_results();
}

std::string get_condensation_engine_tag() {
    return std::string(git::CommitSHA1()) + " (" + std::string(git::Branch()) + ")";
}

EMSCRIPTEN_BINDINGS(capillary_condensation) {
    emscripten::function("run_single_component_capillary_condensation", &run_single_component_capillary_condensation);
    emscripten::function("get_condensation_engine_tag", &get_condensation_engine_tag);

    emscripten::register_vector<SingleComponentCapillaryCondensationRun::Result>("VectorResult");

    emscripten::class_<Component>("Component")
        .constructor<std::string, double, double, double>()
        .property("name_readonly", &Component::get_name)
        .property("density_readonly", &Component::get_density)
        .property("molecular_weight_readonly", &Component::get_molecular_weight)
        .function("p_sat", &Component::get_p_sat)
        .property("molar_volume_readonly", &Component::get_molar_volume);

    emscripten::value_object<SingleComponentCapillaryCondensationRun::Result>("CondensationResult")
        .field("time", &SingleComponentCapillaryCondensationRun::Result::time)
        .field("condensate_volume", &SingleComponentCapillaryCondensationRun::Result::condensate_volume)
        .field("condensate_volume_fraction", &SingleComponentCapillaryCondensationRun::Result::condensate_volume_fraction)
        .field("uniform_to_capillary_ratio", &SingleComponentCapillaryCondensationRun::Result::uniform_to_capillary_ratio)
        .field("capillary_filling_angle", &SingleComponentCapillaryCondensationRun::Result::capillary_filling_angle)
        .field("uniform_coating_thickness", &SingleComponentCapillaryCondensationRun::Result::uniform_coating_thickness);

}
