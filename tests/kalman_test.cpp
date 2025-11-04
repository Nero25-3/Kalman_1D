#include "data_generator.hpp"
#include <gtest/gtest.h>

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

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}





