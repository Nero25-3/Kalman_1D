#pragma once
#include <string>

// Default config constants
constexpr size_t KALMAN_DEF_N = 100;
constexpr double KALMAN_DEF_DT = 0.1;
constexpr double KALMAN_DEF_VELOCITY = 1.5;
constexpr double KALMAN_DEF_NOISE = 0.3;
constexpr double KALMAN_DEF_UNCERTAINTY = 1.0;
constexpr double KALMAN_DEF_PROCESS_VAR = 0.03;
constexpr int    KALMAN_DEF_LOGLEVEL = 1;
const std::string KALMAN_DEF_OUTFILE = "filtered.csv";

// Config struct for pipeline parameters
struct KalmanConfig {
    size_t N;
    double dt;
    double initial_pos;
    double velocity;
    double noise;
    double initial_uncertainty;
    double process_var;
    std::string outfile;
    int loglevel;
};

// Initialize configuration from environment variables or defaults
KalmanConfig get_config_from_env();

// Centralized logging
void print_log(int loglevel, int msglevel, const std::string& msg);
