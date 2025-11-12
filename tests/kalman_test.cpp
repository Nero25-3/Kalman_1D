#include "data_generator.hpp"
#include <gtest/gtest.h>
#include "kalman.hpp"
#include "pipeline.hpp"


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


int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}





