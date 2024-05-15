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
  GLuint eab;
  glm::vec2 window_resolution;
  ShadersConf conf;
  ShadersConf texture_conf;
  GLuint texture;
  GLuint texture_floor;
  GLuint texture_wall;
  GLuint texture_ceiling;
  float fieldOfView;
  glm::mat4 projectionMatrix;
  glm::mat4 viewMatrix;
  glm::mat4 modelMatrix;
  glm::vec3 cameraPosition;
  glm::vec3 cameraDirection;
  glm::vec3 cameraUp;
  float cameraSpeed;
  void prepareCuboid(std::vector<Vertex> &buffer);
  void prepareTriangles(std::vector<Vertex> &buffer);
  void prepareCircle(std::vector<Vertex> &buffer);
  void prepareVbo(const std::vector<Vertex> &buffer, GLuint &vbo);
  void prepareVao(GLuint &vao);
  void prepareEab(GLuint &eab);
  void processInput();
  void prepareTexture(GLuint &texture, std::string filepath, GLint paramWrapS, GLint paramWrapT, GLint paramMinFilter, GLuint paramMagFilter);

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