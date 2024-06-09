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
  ShadersConf g_buffer_conf;
  ShadersConf quad_conf;
  glm::vec2 window_resolution;
  GLuint texture_floor;
  GLuint texture_wall;
  GLuint texture_ceiling;
  GLuint texture_obstacle_bottom;
  GLuint texture_obstacle_up;
  float field_of_view;
  glm::mat4 projection_matrix;
  glm::mat4 view_matrix;
  glm::mat4 model_matrix;
  glm::vec3 camera_position;
  glm::vec3 camera_direction;
  glm::vec3 camera_up;
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
  void prepareCuboid(std::vector<Vertex> &buffer);
  void prepareCircle(std::vector<Vertex> &buffer);
  void prepareQuad(std::vector<Vertex> &buffer);
  void prepareVbo(const std::vector<Vertex> &buffer, GLuint &vbo);
  void prepareVao(GLuint &vao);
  void processInput();
  void prepareTexture(GLuint &texture, std::string filepath, GLint paramWrapS, GLint paramWrapT, GLint paramMinFilter, GLuint paramMagFilter);
  void processMouse(float xoffset, float yoffset);
  void handleMouseMovement(GLFWwindow *window, double xpos, double ypos);
  void prepareShaderUniforms();
  void renderSceneToGbuffer();
  void renderSceneObjectsToGbuffer();
  void prepareDataForQuadRendering();
  void renderQuad();

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