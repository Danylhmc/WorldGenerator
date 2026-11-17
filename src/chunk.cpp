#include "chunk.hpp"
#include<glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/geometric.hpp>
#include <limits>
#include <algorithm>
#include "terrainGenerator.hpp"

Chunk::Chunk(const TerrainGenerator& generator, int resolution, int chunkX, int chunkZ)
  :vao(0),vbo(0),ebo(0),
  generator(generator),
  position(Vec2(chunkX,chunkZ)),
  resolution(resolution){
  generateMesh(resolution);
  uploadMesh();
}

Chunk::~Chunk(){
  if(vao){
    glDeleteVertexArrays(1, &vao);
    vao = 0;
  }
  if(vbo){
    glDeleteBuffers(1, &vbo);
    vbo = 0;
  }
  if(ebo){
    glDeleteBuffers(1, &ebo);
    ebo = 0;
  }
}

void Chunk::rebuildMesh(int newResolution){
  if(newResolution == resolution)
    return;

  glDeleteVertexArrays(1, &vao);
  glDeleteBuffers(1, &vbo);
  glDeleteBuffers(1, &ebo);
  vao = 0;
  vbo = 0;
  ebo = 0;

  generateMesh(newResolution);
  uploadMesh();
  resolution = newResolution;
}

const Vec2& Chunk::getChunkPos()const{
  return position;
}

int Chunk::getResolution()const{
  return resolution;
}

const glm::vec3& Chunk::getBoundsMin()const{
  return boundsMin;
}

const glm::vec3& Chunk::getBoundsMax()const{
  return boundsMax;
}

void Chunk::generateMesh(int resolution){
  int totalVerticesPerAxis = resolution + 1;
  int vertexCount = totalVerticesPerAxis * totalVerticesPerAxis;

  std::vector<glm::vec3> pos(vertexCount);
  std::vector<glm::vec3> norms(vertexCount, glm::vec3(0.0f));

  indices.clear();
  vertices.reserve(totalVerticesPerAxis * totalVerticesPerAxis * 3);

  constexpr float heightScale = 4.0f;

  float yMin = std::numeric_limits<float>::max();
  float yMax = std::numeric_limits<float>::lowest();

  float spacing = static_cast<float>(chunk_width) / resolution;
  int index = 0;
  for (int z = 0; z <= resolution; z ++) {
    for (int x = 0; x <= resolution; x ++) {
      double worldX = static_cast<double>(position.x) * chunk_width + x * spacing;
      double worldZ = static_cast<double>(position.z) * chunk_width + z * spacing;
      float height = static_cast<float>((generator.generateHeight(worldX, worldZ) * 2.0 - 1.0) * heightScale);

      yMin = std::min(yMin, height);
      yMax = std::max(yMax, height);
      pos[index++] = glm::vec3(worldX, height, worldZ);
    }
  }

  int stride = resolution + 1;
  for (int z = 0; z < resolution; z++) {
    for (int x = 0; x < resolution; x ++) {

      unsigned int p00 = z * stride + x;
      unsigned int p10 = z * stride + (x + 1);
      unsigned int p01 = (z + 1) * stride + x;
      unsigned int p11 = (z + 1) * stride + (x + 1);

      {
        glm::vec3 normal = glm::cross(pos[p01] - pos[p00], pos[p10] - pos[p00]);
        norms[p00] += normal;
        norms[p10] += normal;
        norms[p01] += normal;
      }

      {
        glm::vec3 normal = glm::cross(pos[p10] - pos[p11], pos[p01] - pos[p11]);
        norms[p10] += normal;
        norms[p11] += normal;
        norms[p01] += normal;
      }

      indices.push_back(p00);
      indices.push_back(p01);
      indices.push_back(p10);

      indices.push_back(p10);
      indices.push_back(p01);
      indices.push_back(p11);
    }
  }
  vertices.clear();
  vertices.reserve(vertexCount * 6);

  float xMin = position.x * chunk_width;
  float xMax = xMin + chunk_width;
  float zMin = position.z * chunk_width;
  float zMax = zMin + chunk_width;

  boundsMin = glm::vec3(xMin, yMin, zMin);
  boundsMax = glm::vec3(xMax, yMax, zMax);

  for(int i = 0; i < vertexCount; i++){
    glm::vec3 n = glm::normalize(norms[i]);
    vertices.push_back(pos[i].x);
    vertices.push_back(pos[i].y);
    vertices.push_back(pos[i].z);

    vertices.push_back(n.x);
    vertices.push_back(n.y);
    vertices.push_back(n.z);
  }

  indexCount = static_cast<GLsizei>(indices.size());
}

void Chunk::uploadMesh(){
  glGenVertexArrays(1, &vao);
  glGenBuffers(1, &vbo);
  glGenBuffers(1, &ebo);

  glBindVertexArray(vao);

  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);
  
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);

  glVertexAttribPointer(0, 3, GL_FLOAT, false, 6 * sizeof(float), (void*)0);
  glVertexAttribPointer(1, 3, GL_FLOAT, false, 6 * sizeof(float), (void*)(3 * sizeof(float)));
  glEnableVertexAttribArray(0);
  glEnableVertexAttribArray(1);

  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);

}

void Chunk::draw(){
  glBindVertexArray(vao);
  glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, 0);
}
