#pragma once
#include <vector>

/**
 * @brief One-dimensional Kalman filter for position estimation with constant velocity model.
 *
 * Implements standard predict and update steps.
 */
class Kalman1D {
   public:
    /**
     * @brief Construct Kalman filter with initial parameters.
     * @param initial_position Initial estimate of position
     * @param initial_uncertainty Initial estimate uncertainty (variance)
     * @param velocity Constant model velocity
     * @param process_var Process noise variance (model error, Q)
     * @param measurement_var Measurement noise variance (R)
     */
    Kalman1D(double initial_position, double initial_uncertainty, double velocity,
             double process_var, double measurement_var);

    /**
     * @brief Perform prediction step (time update).
     * @param dt Delta time since last update
     */
    void predict(double dt);

    /**
     * @brief Perform update step (measurement update).
     * @param measurement Noisy measurement for current position
     */
    void update(double measurement);

    /**
     * @brief Get current estimated position.
     */
    double get_position() const;

    /**
     * @brief Get current estimate variance.
     */
    double get_uncertainty() const;

   private:
    double position_;         ///< Estimated position
    double velocity_;         ///< Constant velocity
    double uncertainty_;      ///< Estimate uncertainty (variance)
    double process_var_;      ///< Q:Process noise variance, tune for system dynamics
    double measurement_var_;  ///< R:Measurement noise variance, tune for sensor quality
};
