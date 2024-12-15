//
// Created by egor on 12/14/24.
//

#ifndef COMPONENT_H
#define COMPONENT_H

#include <string>
#include <functional>

class Component {
public:
    Component(std::string name, double density, double molecular_weight, std::function<double(double)> p_sat);

    std::string const & get_name() const {
        return name;
    }

    double get_density() const {
        return density;
    }

    double get_molecular_weight() const {
        return molecular_weight;
    }

    double get_p_sat(double temperature) const {
        return p_sat(temperature);
    }

    double get_molar_volume() const {
        return molar_volume;
    }

private:
    const std::string name;
    const double density, molecular_weight;
    const std::function<double(double)> p_sat;
    const double molar_volume;
};

#endif //COMPONENT_H
