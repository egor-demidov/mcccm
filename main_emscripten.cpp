#include <vector>
#include <numeric>

#include <fmt/format.h>
#include <boost/numeric/odeint.hpp>
#include <emscripten/bind.h>

#include "geometry/geometry.h"
#include "condensation/multicomponent_system.h"

static constexpr double gam = 0.15;

void harmonic_oscillator(const std::vector<double> &x , std::vector<double> &dxdt , const double t [[maybe_unused]])
{
    dxdt[0] = x[1];
    dxdt[1] = -x[0] - gam*x[1];
}

void do_stuff() {
    Component comp_1 = Component("comp_1", 1000.0, 20e-3, [](double) {return 1000.0;});
    MulticomponentSystem system(10.0, 0.0, 14e-9, 1.0, [](double) {return 300.0;}, [](double) {return 0.9;}, [](std::vector<double> const &) {return 0.072;}, {comp_1});

    std::vector<double> x(1);
    x[0] = 0.0000001; // start at x=1.0, p=0.0

    size_t steps = boost::numeric::odeint::integrate( system ,
                              x , 0.0 , 1.0 , 0.01 );

    fmt::println("Number of steps: {}", steps);
}

//double get_volume_for_filling_angle(double contact_angle, double filling_angle) {
//    if (contact_angle < generated_geometry.begin_ca || contact_angle > generated_geometry.end_ca ||
//        filling_angle < generated_geometry.begin_fa || filling_angle > generated_geometry.end_fa) [[unlikely]]
//        return std::numeric_limits<double>::quiet_NaN();
//
//    unsigned long ca_lo_index = static_cast<unsigned long>((contact_angle - generated_geometry.begin_ca) * static_cast<double>(Geometry_t::n_ca) / (generated_geometry.end_ca - generated_geometry.begin_ca));
//    fmt::println("{}", ca_lo_index);
//
//    return 0.0;
//}

EMSCRIPTEN_BINDINGS(capillary_condensation) {
    emscripten::function("do_stuff", &do_stuff);
    // emscripten::function("get_volume", &get_volume);
//    emscripten::function("get_volume_for_filling_angle", &get_volume_for_filling_angle);

    emscripten::register_vector<double>("VectorDouble");
//    emscripten::class_<Geometry_t>("Geometry");
}
