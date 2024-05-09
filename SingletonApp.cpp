#include "SingletonApp.h"
#include <iostream>
#include "Data.hpp"

SingletonApp::SingletonApp() : window(nullptr), vao(0), vbo(0), window_resolution(glm::vec2(800, 600)), conf() {}

SingletonApp &SingletonApp::getInstance()
{
  static SingletonApp app;
  return app;
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
  std::vector<Vertex> vertexes_blue = {{positions[0], colors[0]}, {positions[1], colors[1]}, {positions[2], colors[2]}};
  std::vector<Vertex> vertexes_red = {{positions[3], colors[3]}, {positions[4], colors[4]}, {positions[5], colors[5]}};
  std::vector<Vertex> vertexes_green = {{positions[6], colors[6]}, {positions[7], colors[7]}, {positions[8], colors[8]}};
  Triangle triangles[3];
  triangles[0] = Triangle(vertexes_blue);
  triangles[1] = Triangle(vertexes_red);
  triangles[2] = Triangle(vertexes_green);
  std::vector<Vertex> buffer;
  for (size_t i = 0; i < 3; i++)
  {
    buffer.push_back({triangles[i].vertexes[0]});
    buffer.push_back({triangles[i].vertexes[1]});
    buffer.push_back({triangles[i].vertexes[2]});
  }
  return buffer;
}

void SingletonApp::prepareVbo(const std::vector<Vertex> &buffer)
{
  glGenBuffers(1, &this->vbo);

  glBindBuffer(GL_ARRAY_BUFFER, this->vbo);
  glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * buffer.size(), buffer.data(), GL_STATIC_DRAW);
}

void SingletonApp::prepareVao()
{
  glGenVertexArrays(1, &this->vao);
  glBindVertexArray(this->vao);
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
  this->prepareVbo(this->prepareTriangles());
  this->prepareVao();
  return true;
}

SingletonApp::~SingletonApp()
{
  glDeleteVertexArrays(1, &this->vao);
  glDeleteBuffers(1, &this->vbo);
}