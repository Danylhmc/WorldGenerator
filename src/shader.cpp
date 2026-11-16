#include "shader.hpp"
#include <fstream>
#include <glm/gtc/type_ptr.hpp>
#include <sstream>
#include <iostream>
#include <stdexcept>

Shader::Shader(const char* vertex, const char* fragment){
  std::fstream vFile, fFile;
  vFile.open(vertex);
  if(!vFile.is_open()){
    std::runtime_error("Error couldn't open Vertex file\n");
  }

  fFile.open(fragment);
  if(!fFile.is_open()){
    std::runtime_error("Error couldn't open Fragment file\n");
  }

  std::stringstream vStream, fStream;
  vStream << vFile.rdbuf();
  fStream << fFile.rdbuf();

  std::string vString, fString;
  vString = vStream.str();
  fString = fStream.str();

  const char* vCode = vString.c_str();
  const char* fCode = fString.c_str();

  unsigned int Vertex, Fragment;

  Vertex = glCreateShader(GL_VERTEX_SHADER);
  Fragment = glCreateShader(GL_FRAGMENT_SHADER);

  glShaderSource(Vertex, 1, &vCode, NULL);
  glShaderSource(Fragment, 1, &fCode, NULL);

  glCompileShader(Vertex);
  checkForCompilationErrors(Vertex, "Vertex");

  glCompileShader(Fragment);
  checkForCompilationErrors(Fragment, "Fragment");

  ID = glCreateProgram();
  glAttachShader(ID, Vertex);
  glAttachShader(ID, Fragment);

  glLinkProgram(ID);
  char infoLog[1280];
  int success;

  glGetProgramiv(ID, GL_LINK_STATUS, &success);
  if(!success){
    glGetProgramInfoLog(ID, 1280, NULL, infoLog);
    std::cerr << "ERROR LINKING PROGRAM :" << infoLog << "\n";
  }

  glDeleteShader(Vertex);
  glDeleteShader(Fragment);
};

void Shader::checkForCompilationErrors(unsigned int shader, const char* type){
  char infoLog[1280];
  int success;

  glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
  if(!success){
    glGetShaderInfoLog(shader, 1280, NULL, infoLog);
    std::cerr << "ERROR COMPILING SHADER of type "<< type<< ":" << infoLog << "\n";
  }
};

Shader::~Shader(){
    glDeleteProgram(ID);
}

void Shader::use() const{
  glUseProgram(ID);
};

void Shader::setBool(const char* name, int value){
  glUniform1i(glGetUniformLocation(ID, name), value);
};

void Shader::setInt(const char* name, int value){
  glUniform1i(glGetUniformLocation(ID, name), value);
};

void Shader::setFloat(const char* name , float value){
  glUniform1f(glGetUniformLocation(ID, name), value);

};

void Shader::setVec3(const char* name, const glm::vec3& vec){
  glUniform3fv(glGetUniformLocation(ID, name), 1, glm::value_ptr(vec));
}

void Shader::setVec4(const char* name, const glm::vec4& vec){
  glUniform4fv(glGetUniformLocation(ID, name), 1, glm::value_ptr(vec));
};

void Shader::setMat3(const char* name, const glm::mat3& mat){
  glUniformMatrix3fv(glGetUniformLocation(ID, name), 1, false, glm::value_ptr(mat));
};

void Shader::setMat4(const char* name, const glm::mat4& mat){
  glUniformMatrix4fv(glGetUniformLocation(ID, name), 1, false,glm::value_ptr(mat));
};


