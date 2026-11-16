#pragma once 
#include <glm/glm.hpp>
#include <vector>
#include "terrainGenerator.hpp"

class Vec2{
public:
  float x;
  float z;
  Vec2(float x, float z):x(x),z(z){};
};

class Chunk{
public:
  static constexpr float chunk_width = 64.0f;
  static constexpr float chunk_depth = 64.0;

  glm::vec3 boundsMin;
  glm::vec3 boundsMax;

  Chunk(const TerrainGenerator& generator, int resolution, int chunkX, int chunkZ);
  ~Chunk();

  const Vec2& getChunkPos()const;
  int getResolution()const;
  const glm::vec3& getBoundsMin()const;
  const glm::vec3& getBoundsMax()const;

  void rebuildMesh(int newResolution);
  void generateMesh(int resolution);
  void uploadMesh();
  void draw();

private:
  unsigned int vao, vbo, ebo;
  TerrainGenerator generator;
  Vec2 position;
  std::vector<float> vertices;
  std::vector<unsigned int> indices;
  int vertexCount;
  int indexCount;
  int resolution;
};
