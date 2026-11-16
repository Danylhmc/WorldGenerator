#include "world.hpp"
#include <cmath>
#include <memory>

World::World(int seed)
  :generator(seed),shader(Shader("shaders/vertex.glsl", "shaders/fragment.glsl")){
  chunks.reserve(worldSize * worldSize);
  generate();
}

void World::generate(){
  for(int x = 0; x < worldSize; ++x){
    for(int z = 0; z < worldSize; ++z){
      chunks.push_back(std::make_unique<Chunk>(generator, 64, x, z));
    }
  }
}

void World::draw(const glm::mat4& view, const glm::mat4& projection, const glm::vec3& viewPos){
  frustum.update(view, projection);
  shader.use();
  glm::mat4 model(1.0f);
  glm::vec3 lightColor(1.0f, 0.95f, 0.85f);
  glm::vec3 lightPos(100.0f, 50.0f, 50.0f);

  shader.setVec3("lightColor", lightColor);
  shader.setVec3("lightPos", lightPos);
  shader.setVec3("viewPos", viewPos);
  shader.setMat4("model", model);
  shader.setMat4("view", view);
  shader.setMat4("projection", projection);

  for(auto& chunk : chunks){
    if(frustum.isVisible(chunk->getBoundsMin(), chunk->getBoundsMax())){
      chunk->draw();
    }
  }
}


void World::update(const glm::vec3& cameraPos){
  int playerChunkX = (int)std::floor(cameraPos.x / Chunk::chunk_width);
  int playerChunkZ = (int)std::floor(cameraPos.z / Chunk::chunk_width);

  for(auto it = chunks.begin(); it != chunks.end();){
    Chunk* chunk = it->get();
    
    int dx = chunk->getChunkPos().x - playerChunkX;
    int dz = chunk->getChunkPos().z - playerChunkZ;

    if(std::abs(dx) > unloadRadius || std::abs(dz) > unloadRadius){
      it = chunks.erase(it);
    }
    else it++;
  }

  for(int dx = -loadRadius; dx <= loadRadius; dx++){
    for(int dz = -loadRadius; dz <= loadRadius; dz++){
      int chunkX = playerChunkX + dx;
      int chunkZ = playerChunkZ + dz;

      if(!getChunkFromChunkPos(chunkX, chunkZ)){
        int distance = std::max(std::abs(dx), std::abs(dz));
        int desiredResolution;
    
        if(distance < LOD0dist)
          desiredResolution = LOD0;
        else if(distance < LOD1dist)
          desiredResolution = LOD1;
        else
          desiredResolution = LOD2;

        chunks.push_back(std::make_unique<Chunk>(generator, desiredResolution, chunkX, chunkZ));
      }
    }
  }

  for(auto it = chunks.begin(); it != chunks.end(); ++ it){
    Chunk* chunk = it->get();

    int dx = chunk->getChunkPos().x - playerChunkX;
    int dz = chunk->getChunkPos().z - playerChunkZ;

    int distance = std::max(std::abs(dx), std::abs(dz));
    int desiredResolution;
    
    if(distance < LOD0dist)
      desiredResolution = LOD0;
    else if(distance < LOD1dist)
      desiredResolution = LOD1;
    else
      desiredResolution = LOD2;

    if(chunk->getResolution() != desiredResolution)
      chunk->rebuildMesh(desiredResolution);
  }
}

Chunk* World::getChunkFromChunkPos(int chunkX, int chunkZ)const{
  for(auto& chunk : chunks){
    if(chunk->getChunkPos().x == chunkX && chunk->getChunkPos().z == chunkZ)
      return chunk.get();
  }
  return nullptr;
}
