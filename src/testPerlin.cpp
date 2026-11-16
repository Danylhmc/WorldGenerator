#include "perlinNoise.hpp"
#include <fstream>
#include <algorithm>

int main() {
    Perlin perlin(42);

    constexpr int size = 512;
    constexpr double frequency = 0.02;

    std::ofstream image("perlin.pgm");

    image << "P2\n";
    image << size << " " << size << "\n";
    image << "255\n";

    for(int z = 0; z < size; z++) {
        for(int x = 0; x < size; x++) {
            double value = perlin.noise(
                x * frequency,
                z * frequency
            );

            int gray = static_cast<int>(
                std::clamp(value, 0.0, 1.0) * 255.0
            );

            image << gray << " ";
        }
        image << "\n";
    }
}
