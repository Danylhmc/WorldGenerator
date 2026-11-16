#include "frustum.hpp"
#include <glm/geometric.hpp>

void Frustum::update(const glm::mat4 &view, const glm::mat4& projection){
  glm::mat4 M = projection * view;

  glm::vec4 row0(
    M[0][0], M[1][0],
    M[2][0], M[3][0]
  );

  glm::vec4 row1(
    M[0][1], M[1][1],
    M[2][1], M[3][1]
  );

  glm::vec4 row2(
    M[0][2], M[1][2],
    M[2][2], M[3][2]
  );

  glm::vec4 row3(
    M[0][3], M[1][3],
    M[2][3], M[3][3]
  );

  extractedPlanes[0] = row3 + row0;
  extractedPlanes[1] = row3 - row0;
  extractedPlanes[2] = row3 + row1;
  extractedPlanes[3] = row3 - row1;
  extractedPlanes[4] = row3 + row2;
  extractedPlanes[5] = row3 - row2;

  for(int i = 0; i < 6; i++){
    planes[i] = normalizePlane(extractedPlanes[i]);
  }
}

Plane Frustum::normalizePlane(const glm::vec4& plane){
  glm::vec3 normal(plane.x, plane.y, plane.z);

  float length = glm::length(normal);

  if(length == 0.0f)
    return {};

  return {
    normal / length,
    plane.w / length
  };
}

bool Frustum::isOutside(const Plane& plane, const glm::vec3& boundsMin, const glm::vec3& boundsMax){
  glm::vec3 n = plane.normal;
  glm::vec3 pVertex;
  
  if(n.x >= 0)
    pVertex.x = boundsMax.x;
  else if(n.x <= 0)
    pVertex.x = boundsMin.x;

  if(n.y >= 0)
    pVertex.y = boundsMax.y;
  else
    pVertex.y = boundsMin.y;

  if(n.z >= 0)
    pVertex.z = boundsMax.z;
  else if(n.z < 0)
    pVertex.z = boundsMin.z;

  float s = glm::dot(n, pVertex) + plane.distance;

  return s < 0.0f ? true : false;
}

bool Frustum::isVisible(const glm::vec3 &boundsMin, const glm::vec3 &boundsMax){
  for(int i = 0; i < 6; i++){
    if(isOutside(planes[i] , boundsMin, boundsMax))
       return false;
  }
  return true;
}
