cmake_minimum_required(VERSION 3.20)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

project(WorldGenerator)

add_executable(WorldGenerator
  src/main.cpp
  src/shader.cpp
  src/chunk.cpp
  src/world.cpp
  src/frustum.cpp
  src/terrainGenerator.cpp
  external/glad/src/glad.c
)

target_include_directories(WorldGenerator
  PRIVATE
    include
    external/glad/include
)

target_compile_features(WorldGenerator
  PRIVATE
    cxx_std_20
)

find_package(OpenGL REQUIRED)
find_package(glfw3 REQUIRED)
find_package(glm REQUIRED)

target_link_libraries(WorldGenerator
  PRIVATE
  OpenGL::GL
  glfw
  glm::glm-header-only
)
