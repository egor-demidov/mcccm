//
// Created by egor on 12/14/24.
//

#include "multicomponent_system.h"

MulticomponentSystem::MulticomponentSystem(
    double neck_filling_angle,
    double contact_angle,
    double particle_radius,
    double residence_time,
    std::function<double(double)> temperature,
    std::function<double(double)> saturation,
    std::function<double(std::vector<double> const &)> surface_tension,
    std::vector<Component> components
)
    : neck_filling_angle{neck_filling_angle}
    , contact_angle{contact_angle}
    , particle_radius{particle_radius}
    , residence_time{residence_time}
    , temperature{std::move(temperature)}
    , saturation{std::move(saturation)}
    , components{std::move(components)}
    , surface_tension{std::move(surface_tension)}
    , neck_geometry_interpolator{0.0}
    , liquid_geometry_interpolator{contact_angle}
    , num_components{static_cast<unsigned long>(components.size())}
    , neck_volume{neck_geometry_interpolator.interpolate_volume(neck_filling_angle)}
{
}


