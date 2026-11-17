#include "terrainGenerator.hpp"
#include <algorithm>

TerrainGenerator::TerrainGenerator(int seed)
  :regionPerlin(seed),
  plainsPerlin(seed + 1),
  hillsPerlin(seed + 2),
  plateauPerlin(seed + 3),
  mountainPerlin(seed + 4),
  mountainMaskPerlin(seed + 5){}

double TerrainGenerator::sampleFbm(const Perlin& perlin, double worldX, double worldZ, double baseFrequency) {
  double frequency = baseFrequency;
  double amplitude = 1.0;
  double value = 0.0;
  double maxAmplitude = 0.0;

  for (int octave = 0; octave < TerrainGenerator::octaveCount; ++octave) {
    value += (perlin.noise(worldX * frequency, worldZ * frequency) * 2.0 - 1.0) * amplitude;
    maxAmplitude += amplitude;
    frequency *= lacunarity;
    amplitude *= persistence;
  }

  static constexpr double contrast = 1.8;
  double v = value / maxAmplitude;
  return std::clamp(0.5 + 0.5 * v * contrast, 0.0, 1.0);
}

double TerrainGenerator::mountainFbm(double worldX, double worldZ) const {
  double value = 0.0, maxAmplitude = 0.0;
  double frequency = mountainFrequency;
  double amplitude = 1.0;
  double weight = 1.0;

  for (int i = 0; i < mountainOctaves; ++i) {
    double n = 2.0 * mountainPerlin.noise(worldX * frequency, worldZ * frequency) - 1.0;
    double ridge = 1.0 - std::abs(n);
    ridge *= ridge;
    ridge *= weight;
    weight = std::clamp(ridge * 2.0, 0.0, 1.0);

    value += ridge * amplitude;
    maxAmplitude += amplitude;
    frequency *= mountainLacunarity;
    amplitude *= mountainPersistence;
  }
  return value / maxAmplitude;
}

double TerrainGenerator::ridgedNoise(double worldX, double worldZ) const {
  return mountainFbm(worldX, worldZ) * mountainAmplitude;  // 0..amp, sans offset
}

double TerrainGenerator::regionNoise(double worldX, double worldZ) const{
  return regionPerlin.noise(worldX * regionFrequency, worldZ * regionFrequency);
}

double TerrainGenerator::smoothstep(double a, double b, double x) const{
  double t = std::clamp((x - a)/(b - a), 0.0, 1.0);
  return t * t * (3 - 2 * t);
}

double TerrainGenerator::plainsNoise(double worldX, double worldZ) const{
  return (2.0 * sampleFbm(plainsPerlin, worldX, worldZ, plainsFrequency) - 1.0) * plainsAmplitude;
}

double TerrainGenerator::hillsNoise(double worldX, double worldZ) const{
  return (2.0 * sampleFbm(hillsPerlin, worldX, worldZ, hillsFrequency) - 1.0) * hillsAmplitude;
}

double TerrainGenerator::plateauNoise(double worldX, double worldZ) const{
  return (2.0 * sampleFbm(plateauPerlin, worldX, worldZ, plateauFrequency) - 1.0) * plateauAmplitude;
}

double TerrainGenerator::mountainMask(double worldX, double worldZ) const{
  const double noise = mountainMaskPerlin.noise(worldX * mountainMaskFrequency, worldZ * mountainMaskFrequency);
  return smoothstep(mountainMaskMin, mountainMaskMax, noise);
}

double TerrainGenerator::generateHeight(double worldX, double worldZ) const{
  double r = regionNoise(worldX, worldZ);
  
  double s1 = smoothstep(0.35, 0.50, r);
  double s2 = smoothstep(0.55, 0.70, r);

  double plainWeight = 1 - s1;
  double hillWeight = s1 * (1 - s2);
  double plateauWeight = s1 * s2;

  double M = mountainMask(worldX, worldZ);
  double base = plainsNoise(worldX, worldZ) * plainWeight +
              hillsNoise(worldX, worldZ)  * hillWeight +
              plateauNoise(worldX, worldZ)* plateauWeight;
  
  double terrainHeight = base * (1.0 - 0.6 * M) + M * ridgedNoise(worldX, worldZ);

  return 0.5 + terrainHeight / heightToNormalizedScale;
}

double TerrainGenerator::getMountainMaskNoise(double worldX, double worldZ) const{
  return mountainMaskPerlin.noise(
    worldX * mountainMaskFrequency,
    worldZ * mountainMaskFrequency
);
}
