#pragma once
#include "perlinNoise.hpp"

class TerrainGenerator{
public:
  TerrainGenerator(int seed);
  static constexpr double regionFrequency   = 0.0009;
  static constexpr double plainsFrequency   = 0.0020;
  static constexpr double hillsFrequency    = 0.0040;
  static constexpr double plateauFrequency  = 0.0025;
  static constexpr double mountainFrequency = 0.0015;

  static constexpr double plainsAmplitude   = 8.0;
  static constexpr double hillsAmplitude    = 70.0;
  static constexpr double plateauAmplitude  = 90.0;
  static constexpr double mountainAmplitude = 260.0;
  static constexpr double heightToNormalizedScale = 8.0;

  static constexpr int    octaveCount        = 5;
  static constexpr int    mountainOctaves    = 7;
  static constexpr double persistence        = 0.5;
  static constexpr double mountainPersistence= 0.5;
  static constexpr double lacunarity         = 2.0;
  static constexpr double mountainLacunarity = 2.1;

  static constexpr double mountainMaskFrequency = 0.0006;
  static constexpr double mountainMaskMin = 0.45;
  static constexpr double mountainMaskMax = 0.62;

  static double sampleFbm(const Perlin& perlin, double worldX, double worldZ, double baseFrequency);
  double mountainFbm(double worldX, double worldZ) const;
  double regionNoise(double worldX, double worldZ) const;
  double smoothstep(double a, double b, double x) const;
  double plainsNoise(double worldX, double worldZ) const;
  double hillsNoise(double worldX, double worldZ) const;
  double plateauNoise(double worldX, double worldZ) const;
  double ridgedNoise(double worldX, double worldZ) const;
  double generateHeight(double worldX, double worldZ) const;
  double mountainMask(double worldX, double worldZ) const;

  double getMountainMaskNoise(double worldX, double worldZ)const;
private:
  Perlin regionPerlin;
  Perlin plainsPerlin;
  Perlin hillsPerlin;
  Perlin plateauPerlin;
  Perlin mountainPerlin;
  Perlin mountainMaskPerlin;
};
