#pragma once
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Shader{
public:
  Shader(const char* vertex, const char* fragment);
  ~Shader();

  void checkForCompilationErrors(unsigned int shader, const char* type);
  void use() const;
  void setBool(const char* name, int value);
  void setInt(const char* name, int value);
  void setFloat(const char* name , float value);
  void setVec3(const char* name, const glm::vec3& vec);
  void setVec4(const char* name, const glm::vec4& vec);
  void setMat3(const char* name, const glm::mat3& mat);
  void setMat4(const char* name, const glm::mat4& mat);
private:
  unsigned int ID;
};
