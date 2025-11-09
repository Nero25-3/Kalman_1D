#pragma once
#include "data_generator.hpp"
#include "kalman.hpp"
#include <vector>
#include <string>

/**
 * @brief Applies a Kalman filter to a raw data sequence.
 * @param raw Vector of DataPoint (noisy measurements).
 * @param dt Time step between samples.
 * @param kf_proto Prototype Kalman filter (copied for each application, not mutated).
 * @return std::vector<double> Vector of filtered positions.
 */
std::vector<double> apply_kalman(const std::vector<DataPoint>& raw, double dt, const Kalman1D& kf_proto);

/**
 * @brief Exports the raw and filtered data to CSV.
 * @param filename Output CSV file path.
 * @param raw Vector of measurements (with time, position, velocity).
 * @param filtered Vector of filtered positions (same size as raw).
 */
void export_csv(const std::string& filename, const std::vector<DataPoint>& raw, const std::vector<double>& filtered);
