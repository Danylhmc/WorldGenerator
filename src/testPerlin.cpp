#include "terrainGenerator.hpp"
#include <fstream>
#include <algorithm>

int main() {
  TerrainGenerator generator(41);
  int size = 1024;
  std::ofstream image("mountainMask.pgm");

  image << "P2\n";
  image << size << " " << size << "\n";
  image << "255\n";

  for(int z = 0; z < size; z++) {
    for(int x = 0; x < size; x++) {
      double value = generator.getMountainMaskNoise( x, z);
      int gray = static_cast<int>( std::clamp(value, 0.0, 1.0) * 255.0);
      image << gray << " ";
    }
    image << "\n";
  }
}
