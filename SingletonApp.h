#ifndef GKOM2024L_SINGLETONAPP_H
#define GKOM2024L_SINGLETONAPP_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include "ShadersConf.h"
#include "Data.hpp"

class SingletonApp
{
private:
  SingletonApp();
  ~SingletonApp();
  GLFWwindow *window;
  GLuint vao;
  GLuint vbo;
  glm::vec2 window_resolution;
  ShadersConf conf;
  std::vector<Vertex> prepareTriangles();
  std::vector<Vertex> prepareCircles(std::vector<Vertex> &&buffer);
  void prepareVbo(const std::vector<Vertex> &buffer, GLuint &vbo);
  void prepareVao(GLuint &vao);

public:
  SingletonApp(
      const SingletonApp &other_app) = delete;
  SingletonApp &
  operator=(const SingletonApp &other_app) = delete;
  static SingletonApp &getInstance();
  bool prepareWindow();
  bool prepareScene();
  void execute();
};

#endif