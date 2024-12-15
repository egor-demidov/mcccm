//
// Created by egor on 12/14/24.
//

#include "component.h"
#include "../physical_constants.h"

Component::Component(std::string name, double density, double molecular_weight, std::function<double(double)> p_sat)
    : name{std::move(name)}
    , density{density}
    , molecular_weight{molecular_weight}
    , p_sat{std::move(p_sat)}
    , molar_volume{molecular_weight / density}
{}
