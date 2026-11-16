#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/ext/quaternion_geometric.hpp>
#include <glm/geometric.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/trigonometric.hpp>
#include <iostream>
#include "world.hpp"

static int scr_width = 1280;
static int scr_height = 720;

glm::vec3 cameraPos(0.0f);
glm::vec3 cameraFront(0.0f, 0.0f, 1.0f);
glm::vec3 cameraUp(0.0f, 1.0f, 0.0f);

bool firstMouse = true;
float lastX, lastY;
float pitch, yaw;
float sensitivity = 0.08f;

void cursor_callback(GLFWwindow* window, double xpos, double ypos){
  if(firstMouse){
    lastX = (float)xpos;
    lastY = (float)ypos;

    firstMouse = false;
  }

  float xoffset = (float)xpos - lastX;
  float yoffset = lastY - (float)ypos;

  lastX = (float)xpos;
  lastY = (float)ypos;

  xoffset *= sensitivity;
  yoffset *= sensitivity;

  yaw += xoffset;
  pitch += yoffset;

  if(pitch > 89.9f)pitch = 89.9f;
  if(pitch < -89.9f)pitch = -89.9f;

  glm::vec3 direction;
  direction.x = cos(glm::radians(pitch)) * cos(glm::radians(yaw));
  direction.y = sin(glm::radians(pitch));
  direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));

  cameraFront = glm::normalize(direction);
}

float fov = 45.0f;

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset){
  fov -= (float)yoffset;

  if(fov > 90.0f) fov = 90.0f;
  if(fov < 10.0f) fov = 10.0f;
}

void frameBufferSize_callback(GLFWwindow* window, int width, int height){
  glViewport(0, 0, width, height);
  
  scr_width = width;
  scr_height = height;
}

void handle_inputs(GLFWwindow* window){
  if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) glfwSetWindowShouldClose(window, true);

  glm::vec3 right = glm::normalize(glm::cross(cameraFront, cameraUp));
  float speed = 2.0f;

  if(glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) cameraPos += glm::vec3(cameraFront.x, 0.0f, cameraFront.z) * speed;
  if(glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) cameraPos -= glm::vec3(cameraFront.x, 0.0f, cameraFront.z) * speed;
  if(glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) cameraPos += right * speed;
  if(glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) cameraPos -= right * speed;
  if(glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) cameraPos += cameraUp;
  if(glfwGetKey(window, GLFW_KEY_LEFT_ALT) == GLFW_PRESS) cameraPos -= cameraUp;

  if(glfwGetKey(window, GLFW_KEY_F2) == GLFW_PRESS) glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
  if(glfwGetKey(window, GLFW_KEY_F3) == GLFW_PRESS) glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

int main(){
  if(!glfwInit()){
    glfwTerminate();
    std::cerr << "Error Initing glfw\n";
    return -1;
  }

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  GLFWwindow* window = glfwCreateWindow(scr_width, scr_height, "WorldGenerator", NULL, NULL);
  if(!window){
    glfwTerminate();
    std::cerr << "Error creating window\n";
    return -1;
  }

  glfwMakeContextCurrent(window);
  glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
  glfwSetCursorPosCallback(window, cursor_callback);
  glfwSetScrollCallback(window, scroll_callback);
  glfwSetFramebufferSizeCallback(window, frameBufferSize_callback);

  if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)){
    glfwTerminate();
    std::cerr << "Error loading glad\n";
    return -1;
  }

  World world(153);

  glm::vec3 lightColor(1.0f, 0.95f, 0.85f);
  glm::vec3 lightPos(100.0f, 50.0f, 50.0f);

  glEnable(GL_DEPTH_TEST);
  glClearColor(0.5f, 0.6f, 0.75f, 1.0f);

  while(!glfwWindowShouldClose(window)){
    handle_inputs(window);
    glfwPollEvents();

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glm::mat4 view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
    glm::mat4 projection = glm::perspective(glm::radians(fov), (float)scr_width/(float)scr_height, 0.1f, 10000.0f);

    world.update(cameraPos);
    world.draw(view, projection, cameraPos);

    glfwSwapBuffers(window);
  }

  glfwDestroyWindow(window);
  glfwTerminate();

  return 0;
}
