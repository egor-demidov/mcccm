//
// Created by egor on 12/15/24.
//

#ifndef RUN_CONDENSATION_H
#define RUN_CONDENSATION_H

#include <exception>

#include <boost/numeric/odeint.hpp>

class IncorrectNumberOfComponentsException : std::exception {
    const char * what() const noexcept override {
        return "Incorrect number of components in state arrays passed to CondensationRun()";
    }
};

template <typename System>
class CondensationRun {
public:

    struct Observer {
        std::vector<typename System::StateBuffer> & sol_buffer_ref;
        std::vector<double> & t_buffer_ref;

        Observer(std::vector<typename System::StateBuffer> & sol_buffer, std::vector<double> & t_buffer)
            : sol_buffer_ref{sol_buffer}
            , t_buffer_ref{t_buffer}
        {}

        void operator()(typename System::StateBuffer const & sol, double t) {
            sol_buffer_ref.push_back(sol);
            t_buffer_ref.push_back(t);
        }
    };

    CondensationRun(System const & system, typename System::StateBuffer v0, double t_tot, double dt) {
        if (v0.size() != system.get_num_components())
            throw IncorrectNumberOfComponentsException();

        Observer observer(sol_buffer, t_buffer);
        boost::numeric::odeint::runge_kutta4<typename System::StateBuffer> rk;
        n_steps = boost::numeric::odeint::integrate_const(rk, system, v0, 0.0, t_tot, dt, observer);

        // Add the final step to the solution buffer
        observer(v0, t_tot);
    }

    std::vector<typename System::StateBuffer> const & get_solution() const {
        return sol_buffer;
    }

    std::vector<double> const & get_time() const {
        return t_buffer;
    }

    unsigned long get_n_steps() const {
        return n_steps;
    }

    unsigned long get_n_points() const {
        return get_n_steps() + 1ul;
    }

private:
    std::vector<typename System::StateBuffer> sol_buffer;
    std::vector<double> t_buffer;
    unsigned long n_steps;
};

#endif //RUN_CONDENSATION_H
