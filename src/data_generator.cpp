#include "data_generator.hpp"

#include <fstream>

std::vector<DataPoint> generate_noisy_motion(size_t n_steps, double dt, double initial_position,
                                             double velocity, double noise_std) {
    std::vector<DataPoint> out(n_steps); // Preallocate for efficiency
    std::default_random_engine rng{std::random_device{}()};
    std::normal_distribution<double> noise_distr{0.0, noise_std};

    double pos = initial_position;
    for (size_t i = 0; i < n_steps; ++i) {
        double t = i * dt;
        pos += velocity * dt;
        double noisy_pos = pos + noise_distr(rng);
        out[i]= {t, noisy_pos, velocity}; // Store time, noisy position, and velocity
    }
    return out;
}

void write_to_csv(const std::string& filename, const std::vector<DataPoint>& data) {
    std::ofstream file(filename);
    file << "time,position,velocity\n";
    for (const auto& pt : data) {
        file << pt.t << "," << pt.position << "," << pt.velocity << "\n";
    }
}