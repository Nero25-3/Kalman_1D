#pragma once
#include <vector>
#include <random>
#include <string>


// Each datapoint is: time, position, velocity
struct DataPoint {
    double t;
    double position;
    double velocity;
};

/**
 * @brief Generates N synthetic position/velocity samples with Gaussian noise.
 * 
 * @param n_steps Number of steps/samples generated
 * @param dt Sampling interval (time between steps, in seconds)
 * @param initial_position Initial position at t=0
 * @param velocity Constant velocity for all steps
 * @param noise_std Standard deviation of added Gaussian noise
 * @return std::vector<DataPoint> Vector containing simulated time, noisy position, and velocity
 */

std::vector<DataPoint> generate_noisy_motion(
    size_t n_steps, double dt, double initial_position, double velocity,
    double noise_std);


/**
 * @brief Writes a vector of [time, position, velocity] samples to a CSV file.
 */
void write_to_csv(const std::string& filename, const std::vector<DataPoint>& data);