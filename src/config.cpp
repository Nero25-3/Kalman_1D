#include "config.hpp"

#include <yaml-cpp/yaml.h>

#include <filesystem>
#include <fstream>

template <typename T>
void assign_if_exists(const YAML::Node& node, const std::string& key, T& field) {
    if (node[key]) {
        field = node[key].as<T>();
    }
}

KalmanConfig load_config_yaml(const std::string& yamlfile) {
    KalmanConfig conf = {100, 0.1, 0.0, 1.5, 0.3, 1.0, 0.03, 1.0, "filtered.csv", 1, "kalman.log"};
    YAML::Node config = YAML::LoadFile(yamlfile);

    assign_if_exists(config, "N", conf.N);
    assign_if_exists(config, "dt", conf.dt);
    assign_if_exists(config, "velocity", conf.velocity);
    assign_if_exists(config, "noise", conf.noise);
    assign_if_exists(config, "initial_uncertainty", conf.initial_uncertainty);
    assign_if_exists(config, "process_var", conf.process_var);
    assign_if_exists(config, "measurement_var", conf.measurement_var);
    assign_if_exists(config, "outfile", conf.outfile);
    assign_if_exists(config, "loglevel", conf.loglevel);
    assign_if_exists(config, "logfile", conf.logfile);
    assign_if_exists(config, "log_max_size_mb", conf.log_max_size_mb);
    assign_if_exists(config, "log_rotate_count", conf.log_rotate_count);

    return conf;
}

void print_log_file(int loglevel, int msglevel, const std::string& msg, const KalmanConfig& cfg) {
    if (loglevel < msglevel) return;
    rotate_log_if_needed(cfg);
    std::ofstream logstream(cfg.logfile, std::ios::app);
    logstream << msg << std::endl;
}
void rotate_log_if_needed(const KalmanConfig& cfg) {
    namespace fs = std::filesystem;
    if (!fs::exists(cfg.logfile)) return;
    size_t cur_size = fs::file_size(cfg.logfile);
    size_t max_size = static_cast<size_t>(cfg.log_max_size_mb * 1024 * 1024);  // Convert MB → bytes

    if (cur_size < max_size) return;

    for (int i = cfg.log_rotate_count; i >= 1; --i) {
        std::string old = cfg.logfile + "." + std::to_string(i - 1);
        std::string neu = cfg.logfile + "." + std::to_string(i);
        if (fs::exists(neu)) fs::remove(neu);
        if (fs::exists(old)) fs::rename(old, neu);
    }
    fs::rename(cfg.logfile, cfg.logfile + ".0");
    std::ofstream newlog(cfg.logfile, std::ios::trunc);
}
