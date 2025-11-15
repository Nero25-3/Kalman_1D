#include <cstdlib>
#include <iostream>
#include <string>

#include "data_generator.hpp"
#include "kalman.hpp"
#include "pipeline.hpp"
#include "config.hpp"

int main() {
 KalmanConfig cfg = get_config_from_env();

    print_log(cfg.loglevel, 1, "[INFO] Kalman Demo Config:"
        "\n  N=" + std::to_string(cfg.N) +
        "\n  dt=" + std::to_string(cfg.dt) +
        "\n  velocity=" + std::to_string(cfg.velocity) +
        "\n  noise=" + std::to_string(cfg.noise) +
        "\n  initial_uncertainty=" + std::to_string(cfg.initial_uncertainty) +
        "\n  process_var=" + std::to_string(cfg.process_var) +
        "\n  outfile=" + cfg.outfile +
        "\n  loglevel=" + std::to_string(cfg.loglevel));

    auto data = generate_noisy_motion(cfg.N, cfg.dt, cfg.initial_pos, cfg.velocity, cfg.noise);
    Kalman1D kf_proto(cfg.initial_pos, cfg.initial_uncertainty, cfg.velocity, cfg.process_var, cfg.noise);
    auto filtered = apply_kalman(data, cfg.dt, kf_proto);
    export_csv(cfg.outfile, data, filtered);

    print_log(cfg.loglevel, 2, "[DEBUG] First filtered value: " + std::to_string(filtered.front())
        + ", Last filtered value: " + std::to_string(filtered.back()));
    return 0;
}
