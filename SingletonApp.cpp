#include "SingletonApp.h"
#include <iostream>
#include <cmath>
#include "Data.hpp"

SingletonApp::SingletonApp() : window(nullptr), vao(0), vbo(0), window_resolution(glm::vec2(800, 600)), conf() {}

SingletonApp &SingletonApp::getInstance()
{
  static SingletonApp app;
  return app;
}

void FrameBufferSizeChangeCallbackFunction(GLFWwindow *window, int width, int height)
{
  glViewport(0, 0, width, height);
}

bool SingletonApp::prepareWindow()
{
  // Inicjalizacja GLFW
  if (!glfwInit())
  {
    std::cerr << "Błąd inicjalizacji GLFW" << std::endl;
    return false;
  }

  // Konfiguracja GLFW
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);

  // Utwórz okno
  this->window = glfwCreateWindow((int)this->window_resolution.x, (int)this->window_resolution.y, "Scena 3D", nullptr, nullptr);
  if (!window)
  {
    std::cerr << "Błąd tworzenia okna GLFW" << std::endl;
    glfwTerminate();
    return false;
  }

  // Ustaw kontekst OpenGL dla okna
  glfwMakeContextCurrent(window);

  // Inicjalizacja GLEW
  if (glewInit() != GLEW_OK)
  {
    std::cerr << "Błąd inicjalizacji GLEW" << std::endl;
    glfwTerminate();
    return false;
  }

  glfwSetFramebufferSizeCallback(this->window, FrameBufferSizeChangeCallbackFunction);

  return true;
}

std::vector<Vertex> SingletonApp::prepareTriangles()
{
  std::vector<glm::vec3> positions = {glm::vec3(-0.99f, 0.0f, 0.0f), glm::vec3(-0.7f, -0.5f, 0.0f), glm::vec3(-0.7f, 0.5f, 0.0f),
                                      glm::vec3(0.99f, 0.0f, 0.0f), glm::vec3(0.7f, -0.5f, 0.0f), glm::vec3(0.7f, 0.5f, 0.0f),
                                      glm::vec3(-0.7f, 0.5f, 0.0f), glm::vec3(0.7f, 0.5f, 0.0f), glm::vec3(0.0f, 0.8f, 0.0f)};
  std::vector<glm::vec3> colors = {glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.0f, 0.0f, 1.0f),
                                   glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(1.0f, 0.0f, 0.0f),
                                   glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f)};
  std::vector<Vertex> buffer;
  for (size_t i = 0; i < 9; i++)
  {
    buffer.push_back(Vertex(positions[i], colors[i]));
  }
  return buffer;
}

std::vector<Vertex> SingletonApp::prepareCircles(std::vector<Vertex> &&buffer)
{

  glm::vec3 circleMiddle = glm::vec3(0.0f, 0.0f, 0.0f);

  std::vector<glm::vec3> positions;

  positions.push_back(circleMiddle);

  unsigned int segments = 50;

  for (unsigned int i = 0; i <= segments; i++)
  {
    float phi = float(i) / segments * 2.0f * (float)M_PI;

    float x = circleMiddle.x + 0.2f * std::cos(phi);
    float y = circleMiddle.y + 0.2f * std::sin(phi);
    float z = 0.0f;

    positions.push_back(glm::vec3(x, y, z));
  }

  for (size_t i = 0; i < positions.size(); i++)
  {
    buffer.push_back({positions[i], glm::vec3(1.0f, 1.0f, 1.0f)});
  }
  return buffer;
}

void SingletonApp::prepareVbo(const std::vector<Vertex> &buffer, GLuint &vbo)
{
  glGenBuffers(1, &vbo);

  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * buffer.size(), buffer.data(), GL_STATIC_DRAW);
}

void SingletonApp::prepareVao(GLuint &vao)
{
  glGenVertexArrays(1, &vao);
  glBindVertexArray(vao);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)0);
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)(sizeof(glm::vec3)));
  glEnableVertexAttribArray(0);
  glEnableVertexAttribArray(1);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);
}

void SingletonApp::execute()
{
  while (!glfwWindowShouldClose(this->window))
  {
    // Czyszczenie bufora koloru
    glClear(GL_COLOR_BUFFER_BIT);
    this->conf.enable();
    // Narysuj trójkąty
    glBindVertexArray(this->vao);
    glDrawArrays(GL_TRIANGLES, 0, 9);
    glDrawArrays(GL_TRIANGLE_FAN, 9, 59);
    glBindVertexArray(0);
    this->conf.disable();
    // Sprawdź zdarzenia i zamień bufory
    glfwSwapBuffers(this->window);
    glfwPollEvents();
  }
}

bool SingletonApp::prepareScene()
{
  bool status;
  status = this->conf.init();
  if (!status)
  {
    return false;
  }
  this->prepareVbo(this->prepareCircles(this->prepareTriangles()), this->vbo);
  this->prepareVao(this->vao);
  return true;
}

SingletonApp::~SingletonApp()
{
  glDeleteVertexArrays(1, &this->vao);
  glDeleteBuffers(1, &this->vbo);
}