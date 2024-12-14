#include <vector>
#include <numeric>

#include <fmt/format.h>
#include <boost/numeric/odeint.hpp>
#include <emscripten/bind.h>

#include "geometry/geometry.h"

static constexpr double gam = 0.15;

void harmonic_oscillator(const std::vector<double> &x , std::vector<double> &dxdt , const double t [[maybe_unused]])
{
    dxdt[0] = x[1];
    dxdt[1] = -x[0] - gam*x[1];
}

void do_stuff() {
    std::vector<double> x(2);
    x[0] = 1.0; // start at x=1.0, p=0.0
    x[1] = 0.0;

    size_t steps = boost::numeric::odeint::integrate( harmonic_oscillator ,
                              x , 0.0 , 10.0 , 0.1 );

    fmt::println("Number of steps: {}", steps);
}

std::vector<double> get_volume(double contact_angle) {
//    if (contact_angle < generated_geometry.begin_ca || contact_angle > generated_geometry.end_ca)
//        return {};
    unsigned long ca_lo_index = static_cast<unsigned long>((contact_angle - generated_geometry.begin_ca) * static_cast<double>(Geometry_t::n_ca) / (generated_geometry.end_ca - generated_geometry.begin_ca));
    std::vector<double> out {generated_geometry.ca_sets[ca_lo_index].volume.begin(), generated_geometry.ca_sets[ca_lo_index].volume.end()};
    return out;
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
    emscripten::function("get_volume", &get_volume);
//    emscripten::function("get_volume_for_filling_angle", &get_volume_for_filling_angle);

    emscripten::register_vector<double>("VectorDouble");
//    emscripten::class_<Geometry_t>("Geometry");
}
