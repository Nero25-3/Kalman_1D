#include "kalman.hpp"

#include <gtest/gtest.h>

#include <random>

#include "data_generator.hpp"
#include "pipeline.hpp"

TEST(KalmanTest, Dummy) {
    EXPECT_EQ(1, 1);
}

TEST(DataGenerator, OutputSizeAndContent) {
    // 1. Generate 10 samples, dt=0.1, initial position=0, velocity=2, noise=0.2
    size_t N = 10;
    double dt = 0.1, init = 0.0, vel = 2.0, noise = 0.2;
    auto data = generate_noisy_motion(N, dt, init, vel, noise);

    // 2. Output should have N elements
    EXPECT_EQ(data.size(), N);

    // 3. Time should advance correctly (approximate)
    for (size_t i = 0; i < N; ++i) {
        EXPECT_NEAR(data[i].t, i * dt, 1e-9);  // test exact time stamp
        // Noise may make position < or > the expected value, so just check plausible range
        EXPECT_TRUE(std::abs(data[i].position - (init + (i + 1) * vel * dt)) <
                    2.0);  // plausible range
        EXPECT_EQ(data[i].velocity, vel);
    }
}

// Simple Kalman filter test
TEST(Kalman1D, TracksSimpleMeasurement) {
    Kalman1D kf(0.0, 1.0, 1.0, 0.01, 0.2);  // initial position 0, initial uncertainty 1, velocity 1
    kf.predict(1.0);
    kf.update(1.8);  // Measurement much larger than the model prediction
    double estimate = kf.get_position();
    // Estimate must be between 1.0 (model prediction) and 1.8 (measurement)
    EXPECT_GT(estimate, 1.0);
    EXPECT_LT(estimate, 1.8);
}

// Full pipeline test and output size
TEST(Pipeline, ApplyKalmanFull) {
    size_t N = 15;
    auto data = generate_noisy_motion(N, 0.1, 2.0, 1.0, 0.05);
    Kalman1D kf_proto(2.0, 2.0, 1.0, 0.01, 0.05);
    auto filtered = apply_kalman(data, 0.1, kf_proto);
    EXPECT_EQ(filtered.size(), data.size());
    // The first filtered value should be reasonably close to the initial position
    EXPECT_NEAR(filtered.front(), 2.0, 0.5);
}

// CSV export test (header and non-empty line)
#include <fstream>
TEST(Pipeline, ExportCSVCreatesFile) {
    std::vector<DataPoint> in = {{0, 1, 2}, {1, 2, 3}};
    std::vector<double> filt = {10, 20};
    export_csv("testcsv.csv", in, filt);

    std::ifstream inFile("testcsv.csv");
    std::string header;
    getline(inFile, header);
    EXPECT_EQ(header, "time,measured,filtered,velocity");
    std::string line;
    getline(inFile, line);
    EXPECT_FALSE(line.empty());
}

TEST(KalmanFilterSimple, StaticPosition) {
    // Constant measurement at 10.0
    Kalman1D kf(0.0, 1.0, 0.0, 0.01, 0.05);  // Initial position far from the truth
    for (int i = 0; i < 30; ++i) {
        kf.predict(1.0);  // dt, no real movement
        kf.update(10.0);  // always measure 10
    }
    double estimate = kf.get_position();
    EXPECT_NEAR(estimate, 10.0, 0.05);     // filter should converge near 10.0
    EXPECT_LT(kf.get_uncertainty(), 0.2);  // uncertainty should decrease substantially
}

TEST(KalmanFilterSimple, ConstantVelocity) {
    double initial = 0.0;
    double velocity = 1.5;
    Kalman1D kf(initial, 1.0, velocity, 0.01, 0.05);
    double t = 0.0, dt = 1.0;
    for (int i = 0; i < 20; ++i) {
        t += dt;
        double true_pos = initial + velocity * t;
        kf.predict(dt);
        kf.update(true_pos);  // measurement matches the model
    }
    double estimate = kf.get_position();
    double expected = initial + velocity * t;
    EXPECT_NEAR(estimate, expected, 0.05);  // filter should follow perfect motion
    EXPECT_LT(kf.get_uncertainty(), 0.2);
}

TEST(KalmanFilterSimple, ConstantVelocityWithNoise) {
    double initial = 0.0, velocity = 1.0, dt = 1.0;
    Kalman1D kf(initial, 1.0, velocity, 0.01, 0.5);  // high measurement noise
    std::default_random_engine rng(42);
    std::normal_distribution<double> noise(0.0, 0.5);
    for (int i = 0; i < 50; ++i) {
        double true_pos = initial + velocity * (i + 1) * dt;
        double meas = true_pos + noise(rng);
        kf.predict(dt);
        kf.update(meas);
    }
    double final_estimate = kf.get_position();
    double expected = initial + velocity * 50 * dt;
    EXPECT_NEAR(final_estimate, expected, 0.7);  // filter tolerates noise but follows model
}

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
