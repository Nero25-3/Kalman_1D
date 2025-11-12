#include "kalman.hpp"

Kalman1D::Kalman1D(double initial_position, double initial_uncertainty,
                   double velocity, double process_var, double measurement_var)
    : position_(initial_position), velocity_(velocity),
      uncertainty_(initial_uncertainty),
      process_var_(process_var), measurement_var_(measurement_var) {}

void Kalman1D::predict(double dt) {
    position_ += velocity_ * dt;
    uncertainty_ += process_var_;
}

void Kalman1D::update(double measurement) {
    double K = uncertainty_ / (uncertainty_ + measurement_var_);
    position_ += K * (measurement - position_);
    uncertainty_ = (1 - K) * uncertainty_;
}

double Kalman1D::get_position() const {
    return position_;
}

double Kalman1D::get_uncertainty() const {
    return uncertainty_;
}
