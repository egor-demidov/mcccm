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

    // Constructor for constant vapor pressure for web-app
    Component(std::string name, double density, double molecular_weight, double p_sat);

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

namespace components {

    // Real compounds
    extern const Component TEG;     // Triethylene glycol

    // Mosaic condensable components
    namespace MOSAIC {
        extern const Component ARO1;
        extern const Component ARO2;
        extern const Component ALK1;
        extern const Component OLE1;
        extern const Component API1;
        extern const Component API2;
        extern const Component LIM1;
        extern const Component LIM2;
    }

}

#endif //COMPONENT_H
