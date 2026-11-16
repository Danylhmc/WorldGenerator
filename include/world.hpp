#pragma once 
#include <vector>
#include "chunk.hpp"
#include "shader.hpp"
#include "frustum.hpp"
#include "terrainGenerator.hpp"
#include <memory>

class World{
public:
  World(int seed);
  void generate();
  void draw(const glm::mat4& view, const glm::mat4& projection, const glm::vec3& viewPos);
  void update(const glm::vec3& playerPos);
  Chunk* getChunkFromChunkPos(int chunkX, int chunkZ)const;
private:
  const int worldSize = 18;
  const int loadRadius = 18;
  const int unloadRadius = 20;
  const int LOD0 = 64;
  const int LOD1 = 32;
  const int LOD2 = 16;
  const int LOD0dist = 4;
  const int LOD1dist = 8;
  const int LOD2dist = 10;
  TerrainGenerator generator;
  Frustum frustum;
  std::vector< std::unique_ptr<Chunk>> chunks;
  Shader shader;
};
