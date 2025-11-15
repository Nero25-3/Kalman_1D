#pragma once
#include <string>

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
    std::string logfile;
    size_t log_max_size_mb;
    int log_rotate_count;
};

KalmanConfig load_config_yaml(const std::string& yamlfile);

void print_log_file(int loglevel, int msglevel, const std::string& msg, const KalmanConfig& cfg);

void rotate_log_if_needed(const KalmanConfig& cfg);
