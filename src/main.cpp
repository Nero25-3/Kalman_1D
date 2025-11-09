#include "data_generator.hpp"
#include "kalman.hpp"
#include "pipeline.hpp"

int main() {
    const size_t N = 100;
    double dt = 0.1;
    auto data = generate_noisy_motion(N, dt, 0.0, 1.5, 0.3);
    Kalman1D kf_proto(0.0, 1.0, 1.5, 0.03, 0.3);
    auto filtered = apply_kalman(data, dt, kf_proto);
    export_csv("filtered.csv", data, filtered);
    return 0;
}
