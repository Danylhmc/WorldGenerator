#include "terrainGenerator.hpp"
#include <algorithm>

double sampleFbm(const Perlin& perlin, double worldX, double worldZ, double baseFrequency) {
  double frequency = baseFrequency;
  double amplitude = 1.0;
  double value = 0.0;
  double maxAmplitude = 0.0;

  for (int octave = 0; octave < TerrainGenerator::octaveCount; ++octave) {
    value += (perlin.noise(worldX * frequency, worldZ * frequency) * 2.0 - 1.0) * amplitude;
    maxAmplitude += amplitude;
    frequency *= TerrainGenerator::lacunarity;
    amplitude *= TerrainGenerator::persistence;
  }

  return 0.5 + 0.5 * value / maxAmplitude;
}

TerrainGenerator::TerrainGenerator(int seed)
  :regionPerlin(seed),plainsPerlin(seed + 1),hillsPerlin(seed + 2),plateauPerlin(seed + 3){}


double TerrainGenerator::regionNoise(double worldX, double worldZ) const{
  return regionPerlin.noise(worldX * regionFrequency, worldZ * regionFrequency);
}

double TerrainGenerator::smoothstep(double a, double b, double x) const{
  double t = std::clamp((x - a)/(b - a), 0.0, 1.0);
  return t * t * (3 - 2 * t);
}

double TerrainGenerator::plainsNoise(double worldX, double worldZ) const{
  return sampleFbm(plainsPerlin, worldX, worldZ, plainsFrequency) * plainsAmplitude;
}

double TerrainGenerator::hillsNoise(double worldX, double worldZ) const{
  return sampleFbm(hillsPerlin, worldX, worldZ, hillsFrequency) * hillsAmplitude;
}

double TerrainGenerator::plateauNoise(double worldX, double worldZ) const{
  return sampleFbm(plateauPerlin, worldX, worldZ, plateauFrequency) * plateauAmplitude;
}

double TerrainGenerator::generateHeight(double worldX, double worldZ) const{
  double r = regionNoise(worldX, worldZ);
  
  double s1 = smoothstep(0.3, 0.5, r);
  double s2 = smoothstep(0.6, 0.8, r);

  double plainWeight = 1 - s1;
  double hillWeight = s1 * (1 - s2);
  double plateauWeight = s1 * s2;

  return plainsNoise(worldX, worldZ) * plainWeight + hillsNoise(worldX, worldZ) * hillWeight + plateauNoise(worldX, worldZ) * plateauWeight;
}
