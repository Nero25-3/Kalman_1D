#include <algorithm>
#include <chrono>
#include <iostream>
#include <vector>

#include "data_generator.hpp"
#include "kalman.hpp"

/**
 * @brief Runs the Kalman filter on synthetic data and measures per-step latency. *
 * For each prediction and update step, records elapsed time in microseconds.
 * At the end, prints mean, median (p50), p95, and p99 latencies.
 */
void benchmark_kalman_step_latency(size_t N, double dt = 0.1) {
    auto raw = generate_noisy_motion(N, dt, 0.0, 1.0, 0.1);
    Kalman1D kf(0.0, 1.0, 1.0, 0.01, 0.05);

    std::vector<double> latencies;  // Microseconds per step

    for (size_t i = 0; i < raw.size(); ++i) {
        // Measure time before and after each filter step
        auto t_start = std::chrono::high_resolution_clock::now();
        kf.predict(dt);
        kf.update(raw[i].position);
        auto t_end = std::chrono::high_resolution_clock::now();
        double us = std::chrono::duration<double, std::micro>(t_end - t_start).count();
        latencies.push_back(us);
    }

    // Compute latency statistics (mean, percentiles)
    std::sort(latencies.begin(), latencies.end());
    double p50 = latencies[latencies.size() * 0.5];
    double p95 = latencies[latencies.size() * 0.95];
    double p99 = latencies[latencies.size() * 0.99];
    double mean = std::accumulate(latencies.begin(), latencies.end(), 0.0) / latencies.size();

    std::cout << "Bench size: " << N << "\nMean latency: " << mean << " us"
              << "\nMedian (p50): " << p50 << " us" << "\np95: " << p95 << " us" << "\np99: " << p99
              << " us\n";
}

int main() {
    benchmark_kalman_step_latency(100000);  // Large enough for reliable stats
    return 0;
}
