#include "data_generator.hpp"

int main() {
    auto data = generate_noisy_motion(100, 0.1, 0.0, 1.5, 0.3);
    write_to_csv("synthetic.csv", data);
    return 0;
}
