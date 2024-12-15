//
// Created by egor on 12/14/24.
//

#include <cmath>

#include "component.h"
#include "../physical_constants.h"

Component::Component(std::string name, double density, double molecular_weight, std::function<double(double)> p_sat)
    : name{std::move(name)}
    , density{density}
    , molecular_weight{molecular_weight}
    , p_sat{std::move(p_sat)}
    , molar_volume{molecular_weight / density}
{}

double clausius_clapeyron(double t_eval, double t_ref, double p_ref, double delta_h) {
    return p_ref * exp(delta_h / gas_constant * (1.0 / t_ref - 1.0 / t_eval));
}

const Component components::MOSAIC::ARO1(
    "ARO1",
    1000.0,
    150.0e-3,
    [](double temp) -> double {
        return clausius_clapeyron(temp, 298.0, 5.7e-5, 156.0e3);
    }
);

const Component components::MOSAIC::ARO2(
    "ARO2",
    1000.0,
    150.0e-3,
    [](double temp) -> double {
        return clausius_clapeyron(temp, 298.0, 1.6e-3, 156.0e3);
    }
);

const Component components::MOSAIC::ALK1(
    "ALK1",
    1000.0,
    140.0e-3,
    [](double temp) -> double {
        return clausius_clapeyron(temp, 298.0, 5.0e-6, 156.0e3);
    }
);

const Component components::MOSAIC::OLE1(
    "OLE1",
    1000.0,
    140.0e-3,
    [](double temp) -> double {
        return clausius_clapeyron(temp, 298.0, 5.0e-6, 156.0e3);
    }
);

const Component components::MOSAIC::API1(
    "API1",
    1000.0,
    184.0e-3,
    [](double temp) -> double {
        return clausius_clapeyron(temp, 298.0, 4.0e-6, 156.0e3);
    }
);

const Component components::MOSAIC::API2(
    "API2",
    1000.0,
    184.0e-3,
    [](double temp) -> double {
        return clausius_clapeyron(temp, 298.0, 1.7e-4, 156.0e3);
    }
);

const Component components::MOSAIC::LIM1(
    "LIM1",
    1000.0,
    200.0e-3,
    [](double temp) -> double {
        return clausius_clapeyron(temp, 298.0, 2.5e-5, 156.0e3);
    }
);

const Component components::MOSAIC::LIM2(
    "LIM2",
    1000.0,
    200.0e-3,
    [](double temp) -> double {
        return clausius_clapeyron(temp, 298.0, 1.2e-4, 156.0e3);
    }
);

// Unit tests below, compiled only is this is a test target
// available only in a native build
#ifdef DO_TEST

#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_floating_point.hpp>

TEST_CASE("clausius_clapeyron tested", "[clausius_clapeyron]") {
    double ref_temp = 10.0;
    double higher_temp = 12.0;
    double lower_temp = 8.0;
    double p_ref = 1.0;
    double p_at_higher_temp = clausius_clapeyron(higher_temp, ref_temp, p_ref, 1.0e5);
    double p_at_lower_temp = clausius_clapeyron(lower_temp, ref_temp, p_ref, 1.0e5);

    REQUIRE(p_at_higher_temp > p_ref);
    REQUIRE(p_at_lower_temp < p_ref);
}

#endif //DO_TEST
