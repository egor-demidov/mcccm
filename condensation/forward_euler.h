//
// Created by egor on 12/20/24.
//

#ifndef FORWARD_EULER_H
#define FORWARD_EULER_H

template <typename StateType, typename Functor, typename Observer>
long forward_euler(Functor system, StateType const & y0, double start_time, double end_time, double dt, Observer observer) {
    StateType y = y0;  // Use the copy constructor of StateType
    StateType dydt = y0;

    long n_steps = static_cast<long>((end_time - start_time) / dt);

    for (long n = 0; n < n_steps; n ++) {
        system(y, dydt, start_time + static_cast<double>(n) * dt);

        // Integrate
        for (std::pair<typename StateType::iterator, typename StateType::const_iterator> iters = std::make_pair(y.begin(), dydt.begin());
            iters.first != y.end(); ++ iters.first, ++ iters.second) {

            (*iters.first) += (*iters.second) * dt;
        }

        observer(y, start_time + static_cast<double>(n) * dt);
    }

    return n_steps;
}

#endif //FORWARD_EULER_H
