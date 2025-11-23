#include "pipeline.hpp"

#include <fstream>
#include <vector>

std::vector<double> apply_kalman(const std::vector<DataPoint>& raw, double dt,
                                 const Kalman1D& kf_proto) {
    std::vector<double> filtered;
    filtered.reserve(raw.size()); // Reserve space to avoid reallocations
    Kalman1D kf = kf_proto;  // Copy the filter to avoid modifying input
    for (const auto& datapoint : raw) {
        kf.predict(dt);
        kf.update(datapoint.position);
        filtered.push_back(kf.get_position());
    }
    return filtered;
}

void export_csv(const std::string& filename, const std::vector<DataPoint>& raw,
                const std::vector<double>& filtered) {
    std::ofstream out(filename);
    out << "time,measured,filtered,velocity\n";
    for (size_t i = 0; i < raw.size(); ++i) {
        out << raw[i].t << "," << raw[i].position << "," << filtered[i] << "," << raw[i].velocity
            << "\n";
    }
}
