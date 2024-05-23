#ifndef GKOM2024L_SINGLETONAPP_H
#define GKOM2024L_SINGLETONAPP_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include "ShadersConf.h"
#include "Data.hpp"
#include "Framebuffer.h"

class SingletonApp
{
private:
  SingletonApp();
  ~SingletonApp();
  GLFWwindow *window;
  GLuint vao;
  GLuint vbo;
  GLuint eab;
  glm::vec2 window_resolution;
  ShadersConf conf;
  ShadersConf texture_conf;
  GLuint texture;
  GLuint texture_floor;
  GLuint texture_wall;
  GLuint texture_ceiling;
  GLuint texture_obstacle_bottom;
  GLuint texture_obstacle_up;
  float fieldOfView;
  glm::mat4 projectionMatrix;
  glm::mat4 viewMatrix;
  glm::mat4 modelMatrix;
  glm::vec3 cameraPosition;
  glm::vec3 cameraDirection;
  glm::vec3 cameraUp;
  float phi;
  float theta;
  float cameraSpeed;
  float mouse_sensitivity;
  double last_x;
  double last_y;
  bool first_mouse_movement;
  glm::vec3 right;
  GLuint quad_vao;
  GLuint quad_vbo;
  Framebuffer g_buffer;
  ShadersConf g_buffer_conf;
  ShadersConf quad_conf;
  void prepareCuboid(std::vector<Vertex> &buffer);
  void prepareCircle(std::vector<Vertex> &buffer);
  void prepareQuad(std::vector<Vertex> &buffer);
  void prepareVbo(const std::vector<Vertex> &buffer, GLuint &vbo);
  void prepareVao(GLuint &vao);
  void prepareEab(GLuint &eab);
  void processInput();
  void prepareTexture(GLuint &texture, std::string filepath, GLint paramWrapS, GLint paramWrapT, GLint paramMinFilter, GLuint paramMagFilter);
  void processMouse(float xoffset, float yoffset);
  void handleMouseMovement(GLFWwindow *window, double xpos, double ypos);

public:
  SingletonApp(
      const SingletonApp &other_app) = delete;
  SingletonApp &
  operator=(const SingletonApp &other_app) = delete;
  static SingletonApp &getInstance();
  static void mouseCallback(GLFWwindow *window, double xpos, double ypos);
  bool prepareWindow();
  bool prepareScene();
  void execute();
};

#endif