#include <algorithm>
#include <chrono>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#include "data_generator.hpp"
#include "kalman.hpp"

void benchmark_kalman_step_latency(size_t N, double dt = 0.1, int loglevel = 1) {
    auto raw = generate_noisy_motion(N, dt, 0.0, 1.0, 0.1);
    Kalman1D kf(0.0, 1.0, 1.0, 0.01, 0.05);

    std::vector<double> latencies;  // Microseconds per step

    for (size_t i = 0; i < raw.size(); ++i) {
        auto t_start = std::chrono::high_resolution_clock::now();
        kf.predict(dt);
        kf.update(raw[i].position);
        auto t_end = std::chrono::high_resolution_clock::now();
        double us = std::chrono::duration<double, std::micro>(t_end - t_start).count();
        latencies.push_back(us);
    }

    std::sort(latencies.begin(), latencies.end());
    double p50 = latencies[latencies.size() * 0.5];
    double p95 = latencies[latencies.size() * 0.95];
    double p99 = latencies[latencies.size() * 0.99];
    double mean = std::accumulate(latencies.begin(), latencies.end(), 0.0) / latencies.size();

    if (loglevel >= 1) {
        std::cout << "Bench size: " << N << "\nMean latency: " << mean << " us"
                  << "\nMedian (p50): " << p50 << " us" << "\np95: " << p95 << " us"
                  << "\np99: " << p99 << " us\n";
    }
    // Optional: export raw latencies if needed
    const char* env_benchfile = std::getenv("KALMAN_BENCHFILE");
    if (env_benchfile) {
        std::ofstream out(env_benchfile);
        for (double l : latencies) out << l << "\n";
        if (loglevel >= 1) std::cout << "Raw latencies saved to " << env_benchfile << std::endl;
    }
}

int main() {
    const char* env_N = std::getenv("KALMAN_BENCH_SIZE");
    const char* env_dt = std::getenv("KALMAN_BENCH_DT");
    const char* env_loglevel = std::getenv("KALMAN_LOGLEVEL");

    size_t N = env_N ? std::stoul(env_N) : 100000;
    double dt = env_dt ? std::stod(env_dt) : 0.1;
    int loglevel = env_loglevel ? std::stoi(env_loglevel) : 1;

    benchmark_kalman_step_latency(N, dt, loglevel);
    return 0;
}
