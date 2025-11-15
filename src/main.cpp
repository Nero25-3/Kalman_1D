#include <string>

#include "config.hpp"
#include "data_generator.hpp"
#include "kalman.hpp"
#include "pipeline.hpp"

int main(int argc, char* argv[]) {
    std::string config_file = "config.yaml";
    if (argc > 1) {
        config_file = argv[1];
    }
    KalmanConfig cfg = load_config_yaml(config_file);

    print_log_file(cfg.loglevel, 1,
                   "[INFO] Kalman Demo Config:"
                   "\n  N=" +
                       std::to_string(cfg.N) + "\n  dt=" + std::to_string(cfg.dt) +
                       "\n  velocity=" + std::to_string(cfg.velocity) +
                       "\n  noise=" + std::to_string(cfg.noise) +
                       "\n  initial_uncertainty=" + std::to_string(cfg.initial_uncertainty) +
                       "\n  process_var=" + std::to_string(cfg.process_var) + "\n  outfile=" +
                       cfg.outfile + "\n  loglevel=" + std::to_string(cfg.loglevel) +
                       "\n  log_max_size_mb=" + std::to_string(cfg.log_max_size_mb) +
                       "\n  log_rotate_count=" + std::to_string(cfg.log_rotate_count),
                   cfg);

    auto data = generate_noisy_motion(cfg.N, cfg.dt, cfg.initial_pos, cfg.velocity, cfg.noise);
    Kalman1D kf_proto(cfg.initial_pos, cfg.initial_uncertainty, cfg.velocity, cfg.process_var,
                      cfg.noise);
    auto filtered = apply_kalman(data, cfg.dt, kf_proto);
    export_csv(cfg.outfile, data, filtered);

    print_log_file(cfg.loglevel, 2,
                   "[DEBUG] First filtered value: " + std::to_string(filtered.front()) +
                       ", Last filtered value: " + std::to_string(filtered.back()),
                   cfg);
    return 0;
}
