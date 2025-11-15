#include "config.hpp"

#include <yaml-cpp/yaml.h>

#include <filesystem>
#include <fstream>

KalmanConfig load_config_yaml(const std::string& yamlfile) {
    KalmanConfig conf = {100, 0.1, 0.0, 1.5, 0.3, 1.0, 0.03, "filtered.csv", 1, "kalman.log"};
    YAML::Node config = YAML::LoadFile(yamlfile);

    if (config["N"]) conf.N = config["N"].as<size_t>();
    if (config["dt"]) conf.dt = config["dt"].as<double>();
    if (config["velocity"]) conf.velocity = config["velocity"].as<double>();
    if (config["noise"]) conf.noise = config["noise"].as<double>();
    if (config["initial_uncertainty"])
        conf.initial_uncertainty = config["initial_uncertainty"].as<double>();
    if (config["process_var"]) conf.process_var = config["process_var"].as<double>();
    if (config["outfile"]) conf.outfile = config["outfile"].as<std::string>();
    if (config["loglevel"]) conf.loglevel = config["loglevel"].as<int>();
    if (config["logfile"]) conf.logfile = config["logfile"].as<std::string>();
    if (config["log_max_size_mb"]) conf.log_max_size_mb = config["log_max_size_mb"].as<double>();
    if (config["log_rotate_count"]) conf.log_rotate_count = config["log_rotate_count"].as<int>();

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
