#include "data_generator.hpp"
#include <gtest/gtest.h>
#include "kalman.hpp"
#include "pipeline.hpp"
#include <random>


TEST(KalmanTest, Dummy) {
    EXPECT_EQ(1, 1);
}

TEST(DataGenerator, OutputSizeAndContent) {
    // 1. Paramos con 10 pasos, dt=0.1, pos inicial=0, velocidad=2, ruido=0.2
    size_t N = 10;
    double dt = 0.1, init = 0.0, vel = 2.0, noise = 0.2;
    auto data = generate_noisy_motion(N, dt, init, vel, noise);

    // 2. Output debe tener N elementos
    EXPECT_EQ(data.size(), N);

    // 3. El tiempo debe avanzar correctamente (aproximado)
    for (size_t i = 0; i < N; ++i) {
        EXPECT_NEAR(data[i].t, i*dt, 1e-9); // tiempo exacto
        // El ruido puede darte posiciones < o >, así que solo prueba el tipo/dominio
        EXPECT_TRUE(std::abs(data[i].position - (init + (i+1)*vel*dt)) < 2.0); // rango plausible
        EXPECT_EQ(data[i].velocity, vel);
    }
}


// Test simple del filtro de Kalman
TEST(Kalman1D, TracksSimpleMeasurement) {
    Kalman1D kf(0.0, 1.0, 1.0, 0.01, 0.2); // posición inicial 0, incertidumbre inicial 1, velocidad 1
    kf.predict(1.0);
    kf.update(1.8); // Medición bastante más alta que el modelo
    double estimate = kf.get_position();
    // Debe estar entre 1.0 (lo que predijo el modelo) y 1.8 (medición)
    EXPECT_GT(estimate, 1.0);
    EXPECT_LT(estimate, 1.8);
}

// Test del pipeline completo y output size
TEST(Pipeline, ApplyKalmanFull) {
    size_t N = 15;
    auto data = generate_noisy_motion(N, 0.1, 2.0, 1.0, 0.05);
    Kalman1D kf_proto(2.0, 2.0, 1.0, 0.01, 0.05);
    auto filtered = apply_kalman(data, 0.1, kf_proto);
    EXPECT_EQ(filtered.size(), data.size());
    // El primer valor filtrado debe estar razonablemente cerca del inicial
    EXPECT_NEAR(filtered.front(), 2.0, 0.5);
}

// Test de exportación a CSV (cabecera y línea no vacía)
#include <fstream>
TEST(Pipeline, ExportCSVCreatesFile) {
    std::vector<DataPoint> in = {{0,1,2},{1,2,3}};
    std::vector<double> filt = { 10, 20 };
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
    // Medición constante en 10.0
    Kalman1D kf(0.0, 1.0, 0.0, 0.01, 0.05); // Posición inicial lejos de la verdad
    for (int i = 0; i < 30; ++i) {
        kf.predict(1.0);           // dt sin movimiento real
        kf.update(10.0);           // medición siempre 10
    }
    double estimate = kf.get_position();
    EXPECT_NEAR(estimate, 10.0, 0.05);    // el filtro debe converger cerca de 10.0
    EXPECT_LT(kf.get_uncertainty(), 0.2); // la incertidumbre debe reducirse bastante
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
        kf.update(true_pos);              // la medición coincide con el modelo
    }
    double estimate = kf.get_position();
    double expected = initial + velocity * t;
    EXPECT_NEAR(estimate, expected, 0.05);     // el filtro sigue el movimiento perfectamente
    EXPECT_LT(kf.get_uncertainty(), 0.2);
}

TEST(KalmanFilterSimple, ConstantVelocityWithNoise) {
    double initial = 0.0, velocity = 1.0, dt = 1.0;
    Kalman1D kf(initial, 1.0, velocity, 0.01, 0.5); // ruido de medición alto
    std::default_random_engine rng(42);
    std::normal_distribution<double> noise(0.0, 0.5);
    for (int i = 0; i < 50; ++i) {
        double true_pos = initial + velocity * (i+1) * dt;
        double meas = true_pos + noise(rng);
        kf.predict(dt);
        kf.update(meas);
    }
    double final_estimate = kf.get_position();
    double expected = initial + velocity * 50 * dt;
    EXPECT_NEAR(final_estimate, expected, 0.7); // tolera ruido, pero sigue el modelo
}



int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}





