//
// Created by egor on 12/14/24.
//

#include <vector>
#include <numeric>

#include <fmt/format.h>
#include <boost/numeric/odeint.hpp>

#include "geometry/geometry.h"
#include "condensation/multicomponent_system.h"

int main() {
    auto p_sat = [](double) {
        return 10000.0;
    };



    Component comp_1 = Component("comp_1", 1000.0, 20e-3, p_sat);
    MulticomponentSystem system(10.0, 0.0, 14e-9, 10.0, [](double) {return 300.0;}, [](double) {return 1.0;}, [](std::vector<double> const &) {return 0.072;}, {comp_1});

    std::vector<double> x(1);
    x[0] = 0.0; // start at x=1.0, p=0.0

    size_t steps = boost::numeric::odeint::integrate( system ,
                              x , 0.0 , 1.0 , 0.0001 );

    fmt::println("Number of steps, x: {}, {}", steps, x[0]);

    return 0;
}
