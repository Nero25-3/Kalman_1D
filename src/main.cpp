#include <cstdlib>
#include <iostream>
#include <string>

#include "data_generator.hpp"
#include "kalman.hpp"
#include "pipeline.hpp"

int main() {
    // Read environment variables with defaults
    const char* env_N = std::getenv("KALMAN_N");
    const char* env_dt = std::getenv("KALMAN_DT");
    const char* env_noise = std::getenv("KALMAN_NOISE");
    const char* env_velocity = std::getenv("KALMAN_VELOCITY");
    const char* env_datafile = std::getenv("KALMAN_OUTFILE");
    const char* env_loglevel = std::getenv("KALMAN_LOGLEVEL");

    size_t N = env_N ? std::stoul(env_N) : 100;
    double dt = env_dt ? std::stod(env_dt) : 0.1;
    double initial_pos = 0.0;
    double velocity = env_velocity ? std::stod(env_velocity) : 1.5;
    double noise = env_noise ? std::stod(env_noise) : 0.3;
    std::string outfile = env_datafile ? env_datafile : "filtered.csv";
    int loglevel = env_loglevel ? std::stoi(env_loglevel) : 1;  // 1=info, 2=debug

    if (loglevel >= 1)
        std::cout << "[INFO] N=" << N << ", dt=" << dt << ", velocity=" << velocity
                  << ", noise=" << noise << ", output=" << outfile << std::endl;

    auto data = generate_noisy_motion(N, dt, initial_pos, velocity, noise);
    Kalman1D kf_proto(initial_pos, 1.0, velocity, 0.03, noise);
    auto filtered = apply_kalman(data, dt, kf_proto);
    export_csv(outfile, data, filtered);

    if (loglevel >= 2)
        std::cout << "[DEBUG] First filtered value: " << filtered.front()
                  << " Last filtered value: " << filtered.back() << std::endl;
    return 0;
}
