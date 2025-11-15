#include "config.hpp"
#include <cstdlib>
#include <map>
#include <functional>
#include <iostream>

KalmanConfig get_config_from_env() {
    KalmanConfig conf = {
        KALMAN_DEF_N, KALMAN_DEF_DT, 0.0,
        KALMAN_DEF_VELOCITY, KALMAN_DEF_NOISE,
        KALMAN_DEF_UNCERTAINTY, KALMAN_DEF_PROCESS_VAR,
        KALMAN_DEF_OUTFILE, KALMAN_DEF_LOGLEVEL
    };
    const std::map<std::string, std::function<void(const char*)>> setters = {
        {"KALMAN_N",            [&](const char* v){ if(v) conf.N = std::stoul(v); }},
        {"KALMAN_DT",           [&](const char* v){ if(v) conf.dt = std::stod(v); }},
        {"KALMAN_VELOCITY",     [&](const char* v){ if(v) conf.velocity = std::stod(v); }},
        {"KALMAN_NOISE",        [&](const char* v){ if(v) conf.noise = std::stod(v); }},
        {"KALMAN_OUTFILE",      [&](const char* v){ if(v) conf.outfile = v; }},
        {"KALMAN_LOGLEVEL",     [&](const char* v){ if(v) conf.loglevel = std::stoi(v); }},
        {"KALMAN_UNCERTAINTY",  [&](const char* v){ if(v) conf.initial_uncertainty = std::stod(v); }},
        {"KALMAN_PROCESS_VAR",  [&](const char* v){ if(v) conf.process_var = std::stod(v); }},
    };
    for (const auto& kv : setters) kv.second(std::getenv(kv.first.c_str()));
    return conf;
}

void print_log(int loglevel, int msglevel, const std::string& msg) {
    if (loglevel >= msglevel) std::cout << msg << std::endl;
}
