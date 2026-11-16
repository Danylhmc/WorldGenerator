#pragma once 
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

struct Plane{
  glm::vec3 normal;
  float distance;
};

class Frustum{
public:
  void update(const glm::mat4& view, const glm::mat4& projection);
  Plane normalizePlane(const glm::vec4& plane);
  bool isOutside(const Plane& plane, const glm::vec3& boundsMin, const glm::vec3& boundsMax);
  bool isVisible(const glm::vec3& boundsMin, const glm::vec3& boundsMax);
private:
  Plane planes[6];
  glm::vec4 extractedPlanes[6];
};
