#pragma once
#include "perlinNoise.hpp"

class TerrainGenerator{
public:
  TerrainGenerator(int seed);
  static constexpr double regionFrequency = 0.0008;
  static constexpr double plainsFrequency = 0.001;
  static constexpr double hillsFrequency = 0.0015;
  static constexpr double plateauFrequency = 0.0008;
  static constexpr double plainsAmplitude = 15.0;
  static constexpr double hillsAmplitude = 80.0;
  static constexpr double plateauAmplitude = 120.0;
  static constexpr int octaveCount = 6;
  static constexpr double persistence = 0.5;
  static constexpr double lacunarity = 2.0;
  double regionNoise(double worldX, double worldZ) const;
  double smoothstep(double a, double b, double x) const;
  double plainsNoise(double worldX, double worldZ) const;
  double hillsNoise(double worldX, double worldZ) const;
  double plateauNoise(double worldX, double worldZ) const;
  double generateHeight(double worldX, double worldZ) const;
private:
  Perlin regionPerlin;
  Perlin plainsPerlin;
  Perlin hillsPerlin;
  Perlin plateauPerlin;
};
