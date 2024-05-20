#include "SingletonApp.h"
#include <iostream>
#include <cmath>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include "Data.hpp"

SingletonApp::SingletonApp() : window(nullptr), vao(0), vbo(0), window_resolution(glm::vec2(800, 600)), conf(std::string("#version 330 core\n") +
                                                                                                                 std::string("layout (location = 0) in vec3 attrPosition;\n") +
                                                                                                                 std::string("layout (location = 1) in vec3 attrColor;\n") +
                                                                                                                 std::string("out vec3 fragColor;\n") +
                                                                                                                 std::string("uniform mat4 uProjectionMatrix;\n") +
                                                                                                                 std::string("uniform mat4 uViewMatrix;\n") +
                                                                                                                 std::string("uniform mat4 uModelMatrix;\n") +
                                                                                                                 std::string("void main()\n") +
                                                                                                                 std::string("{\n") +
                                                                                                                 std::string("    fragColor = attrColor;\n") +
                                                                                                                 std::string("    gl_Position = uProjectionMatrix * uViewMatrix * uModelMatrix * vec4(attrPosition, 1.0);\n") +
                                                                                                                 std::string("}\n"),
                                                                                                             std::string("#version 330\n") +
                                                                                                                 std::string("in vec3 fragColor;\n") +
                                                                                                                 std::string("out vec4 finalColor;\n") +
                                                                                                                 std::string("void main() {\n") +
                                                                                                                 std::string("   finalColor = vec4(fragColor, 1.0);\n") +
                                                                                                                 std::string("}\n")),
                               texture_conf(std::string("#version 330 core\n") +
                                                std::string("layout (location = 0) in vec3 attrPosition;\n") +
                                                std::string("layout (location = 3) in vec2 attrTexCoord;\n") +
                                                std::string("out vec2 texCoord;\n") +
                                                std::string("uniform mat4 uProjectionMatrix;\n") +
                                                std::string("uniform mat4 uViewMatrix;\n") +
                                                std::string("uniform mat4 uModelMatrix;\n") +
                                                std::string("uniform sampler2D uTexture;\n") +
                                                std::string("void main()\n") +
                                                std::string("{\n") +
                                                std::string("    texCoord = attrTexCoord;\n") +
                                                std::string("    gl_Position = uProjectionMatrix * uViewMatrix * uModelMatrix * vec4(attrPosition, 1.0);\n") +
                                                std::string("}\n"),
                                            std::string("#version 330\n") +
                                                std::string("in vec2 texCoord;\n") +
                                                std::string("out vec4 finalColor;\n") +
                                                std::string("uniform sampler2D uTexture;\n") +
                                                std::string("void main() {\n") +
                                                std::string("   vec4 texColor = texture(uTexture, texCoord);\n") +
                                                std::string("   finalColor = vec4(texColor.xyz, 1.0);\n") +
                                                std::string("}\n"))
{
  fieldOfView = 45;
  cameraPosition = glm::vec3(-19, 0, 10);
  cameraDirection = glm::vec3(1, 0, 0);
  cameraUp = glm::vec3(0, 0, 1);
  cameraSpeed = 0.01f;
  this->first_mouse_movement = true;
  this->mouse_sensitivity = 0.2f;
  this->phi = 0.0f;
  this->theta = 90.0f;
}

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
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

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

void SingletonApp::prepareCuboid(std::vector<Vertex> &buffer)
{
  std::vector<glm::vec3> positions = {glm::vec3(-20.0f, -20.0f, 0.0f), glm::vec3(-20.0f, 20.0f, 0.0f), glm::vec3(20.0f, 20.0f, 0.0f),
                                      glm::vec3(-20.0f, -20.0f, 0.0f), glm::vec3(20.0f, -20.0f, 0.0f), glm::vec3(20.0f, 20.0f, 0.0f),
                                      glm::vec3(-20.0f, -20.0f, 0.0f), glm::vec3(20.0f, -20.0f, 0.0f), glm::vec3(20.0f, -20.0f, 20.0f),
                                      glm::vec3(-20.0f, -20.0f, 0.0f), glm::vec3(-20.0f, -20.0f, 20.0f), glm::vec3(20.0f, -20.0f, 20.0f),
                                      glm::vec3(-20.0f, 20.0f, 0.0f), glm::vec3(20.0f, 20.0f, 0.0f), glm::vec3(20.0f, 20.0f, 20.0f),
                                      glm::vec3(-20.0f, 20.0f, 0.0f), glm::vec3(-20.0f, 20.0f, 20.0f), glm::vec3(20.0f, 20.0f, 20.0f),
                                      glm::vec3(-20.0f, -20.0f, 0.0f), glm::vec3(-20.0f, 20.0f, 0.0f), glm::vec3(-20.0f, 20.0f, 20.0f),
                                      glm::vec3(-20.0f, -20.0f, 0.0f), glm::vec3(-20.0f, -20.0f, 20.0f), glm::vec3(-20.0f, 20.0f, 20.0f),
                                      glm::vec3(20.0f, -20.0f, 0.0f), glm::vec3(20.0f, 20.0f, 0.0f), glm::vec3(20.0f, 20.0f, 20.0f),
                                      glm::vec3(20.0f, -20.0f, 0.0f), glm::vec3(20.0f, -20.0f, 20.0f), glm::vec3(20.0f, 20.0f, 20.0f),
                                      glm::vec3(-20.0f, -20.0f, 20.0f), glm::vec3(-20.0f, 20.0f, 20.0f), glm::vec3(20.0f, 20.0f, 20.0f),
                                      glm::vec3(-20.0f, -20.0f, 20.0f), glm::vec3(20.0f, -20.0f, 20.0f), glm::vec3(20.0f, 20.0f, 20.0f)};
  std::vector<glm::vec3> colors = {glm::vec3(0.0f, 1.0f, 1.0f), glm::vec3(0.0f, 1.0f, 1.0f), glm::vec3(0.0f, 1.0f, 1.0f),
                                   glm::vec3(0.0f, 1.0f, 1.0f), glm::vec3(0.0f, 1.0f, 1.0f), glm::vec3(0.0f, 1.0f, 1.0f),
                                   glm::vec3(0.0f, 1.0f, 1.0f), glm::vec3(0.0f, 1.0f, 1.0f), glm::vec3(0.0f, 1.0f, 1.0f),
                                   glm::vec3(0.0f, 1.0f, 1.0f), glm::vec3(0.0f, 1.0f, 1.0f), glm::vec3(0.0f, 1.0f, 1.0f),
                                   glm::vec3(0.0f, 1.0f, 1.0f), glm::vec3(0.0f, 1.0f, 1.0f), glm::vec3(0.0f, 1.0f, 1.0f),
                                   glm::vec3(0.0f, 1.0f, 1.0f), glm::vec3(0.0f, 1.0f, 1.0f), glm::vec3(0.0f, 1.0f, 1.0f),
                                   glm::vec3(0.0f, 1.0f, 1.0f), glm::vec3(0.0f, 1.0f, 1.0f), glm::vec3(0.0f, 1.0f, 1.0f),
                                   glm::vec3(0.0f, 1.0f, 1.0f), glm::vec3(0.0f, 1.0f, 1.0f), glm::vec3(0.0f, 1.0f, 1.0f),
                                   glm::vec3(0.0f, 1.0f, 1.0f), glm::vec3(0.0f, 1.0f, 1.0f), glm::vec3(0.0f, 1.0f, 1.0f),
                                   glm::vec3(0.0f, 1.0f, 1.0f), glm::vec3(0.0f, 1.0f, 1.0f), glm::vec3(0.0f, 1.0f, 1.0f),
                                   glm::vec3(0.0f, 1.0f, 1.0f), glm::vec3(0.0f, 1.0f, 1.0f), glm::vec3(0.0f, 1.0f, 1.0f),
                                   glm::vec3(0.0f, 1.0f, 1.0f), glm::vec3(0.0f, 1.0f, 1.0f), glm::vec3(0.0f, 1.0f, 1.0f)};
  std::vector<glm::vec3> normals = {glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.0f, 0.0f, 1.0f),
                                    glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.0f, 0.0f, 1.0f),
                                    glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f),
                                    glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f),
                                    glm::vec3(0.0f, -1.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f),
                                    glm::vec3(0.0f, -1.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f),
                                    glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(1.0f, 0.0f, 0.0f),
                                    glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(1.0f, 0.0f, 0.0f),
                                    glm::vec3(-1.0f, 1.0f, 0.0f), glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec3(-1.0f, 0.0f, 0.0f),
                                    glm::vec3(-1.0f, 1.0f, 0.0f), glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec3(-1.0f, 0.0f, 0.0f),
                                    glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, 0.0f, -1.0f),
                                    glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, 0.0f, -1.0f)};
  std::vector<glm::vec2> tex2d_coords = {glm::vec2(0.0f, 0.0f), glm::vec2(0.0f, 1.0f), glm::vec2(1.0f, 1.0f),
                                         glm::vec2(0.0f, 0.0f), glm::vec2(1.0f, 0.0f), glm::vec2(1.0f, 1.0f),
                                         glm::vec2(0.0f, 0.0f), glm::vec2(1.0f, 0.0f), glm::vec2(1.0f, 1.0f),
                                         glm::vec2(0.0f, 0.0f), glm::vec2(0.0f, 1.0f), glm::vec2(1.0f, 1.0f),
                                         glm::vec2(0.0f, 0.0f), glm::vec2(1.0f, 0.0f), glm::vec2(1.0f, 1.0f),
                                         glm::vec2(0.0f, 0.0f), glm::vec2(0.0f, 1.0f), glm::vec2(1.0f, 1.0f),
                                         glm::vec2(0.0f, 0.0f), glm::vec2(1.0f, 0.0f), glm::vec2(1.0f, 1.0f),
                                         glm::vec2(0.0f, 0.0f), glm::vec2(0.0f, 1.0f), glm::vec2(1.0f, 1.0f),
                                         glm::vec2(0.0f, 0.0f), glm::vec2(1.0f, 0.0f), glm::vec2(1.0f, 1.0f),
                                         glm::vec2(0.0f, 0.0f), glm::vec2(0.0f, 1.0f), glm::vec2(1.0f, 1.0f),
                                         glm::vec2(0.0f, 0.0f), glm::vec2(0.0f, 1.0f), glm::vec2(1.0f, 1.0f),
                                         glm::vec2(0.0f, 0.0f), glm::vec2(1.0f, 0.0f), glm::vec2(1.0f, 1.0f)};
  for (size_t i = 0; i < 36; i++)
  {
    buffer.push_back(Vertex(positions[i], colors[i], normals[i], tex2d_coords[i]));
  }
}

void SingletonApp::prepareTriangles(std::vector<Vertex> &buffer)
{
  std::vector<glm::vec3> positions = {glm::vec3(-0.99f, 0.0f, 0.01f), glm::vec3(-0.7f, -0.5f, 0.01f), glm::vec3(-0.7f, 0.5f, 0.01f),
                                      glm::vec3(0.99f, 0.0f, 0.01f), glm::vec3(0.7f, -0.5f, 0.01f), glm::vec3(0.7f, 0.5f, 0.01f),
                                      glm::vec3(-0.7f, 0.5f, 0.01f), glm::vec3(0.7f, 0.5f, 0.01f), glm::vec3(0.0f, 0.8f, 0.01f)};
  std::vector<glm::vec3> colors = {glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.0f, 0.0f, 1.0f),
                                   glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(1.0f, 0.0f, 0.0f),
                                   glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f)};
  std::vector<glm::vec3> normals = {glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.0f, 0.0f, 1.0f),
                                    glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.0f, 0.0f, 1.0f),
                                    glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.0f, 0.0f, 1.0f)};
  std::vector<glm::vec2> tex2d_coords = {glm::vec2(0.0f, 0.0f), glm::vec2(0.0f, 0.0f), glm::vec2(0.0f, 0.0f),
                                         glm::vec2(0.0f, 0.0f), glm::vec2(0.0f, 0.0f), glm::vec2(0.0f, 0.0f),
                                         glm::vec2(0.0f, 0.0f), glm::vec2(0.0f, 1.0f), glm::vec2(1.0f, 1.0f)};
  for (size_t i = 0; i < 9; i++)
  {
    buffer.push_back(Vertex(positions[i], colors[i], normals[i], tex2d_coords[i]));
  }
}

void SingletonApp::prepareCircle(std::vector<Vertex> &buffer)
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
    float z = 0.01f;

    positions.push_back(glm::vec3(x, y, z));
  }

  unsigned int iter = 0;
  for (size_t i = 0; i < positions.size(); i++)
  {
    std::vector<glm::vec2> tex_coords = {glm::vec2(0.0f, 0.0f), glm::vec2(0.0f, 1.0f), glm::vec2(1.0f, 1.0f)};
    buffer.push_back(Vertex(positions[i], glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(0.0f, 0.0f, 1.0f), tex_coords[iter]));
    iter += 1;
    iter = iter % 3;
  }
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
  glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)(2 * sizeof(glm::vec3)));
  glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)(3 * sizeof(glm::vec3)));
  glEnableVertexAttribArray(0);
  glEnableVertexAttribArray(1);
  glEnableVertexAttribArray(2);
  glEnableVertexAttribArray(3);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);
}

void SingletonApp::prepareEab(GLuint &eab)
{
  std::vector<GLshort> vertex_queue;
  vertex_queue.push_back(0);
  vertex_queue.push_back(1);
  vertex_queue.push_back(2);
  vertex_queue.push_back(3);
  vertex_queue.push_back(4);
  vertex_queue.push_back(5);
  vertex_queue.push_back(6);
  vertex_queue.push_back(7);
  vertex_queue.push_back(8);
  vertex_queue.push_back(9);
  vertex_queue.push_back(10);
  vertex_queue.push_back(11);
  glGenBuffers(1, &eab);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, eab);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, vertex_queue.size() * sizeof(GLshort), vertex_queue.data(), GL_STATIC_DRAW);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void SingletonApp::processInput()
{
  if (glfwGetKey(this->window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
  {
    glfwSetWindowShouldClose(this->window, true);
  }

  // Camera movement

  if (glfwGetKey(this->window, GLFW_KEY_W) == GLFW_PRESS)
  {
    // Move forward
    cameraPosition.x += cameraSpeed;
    cameraPosition.x = std::min(cameraPosition.x, 19.0f);
  }

  if (glfwGetKey(this->window, GLFW_KEY_S) == GLFW_PRESS)
  {
    // Move backward
    cameraPosition.x -= cameraSpeed;
    cameraPosition.x = std::max(cameraPosition.x, -19.0f);
  }

  if (glfwGetKey(this->window, GLFW_KEY_A) == GLFW_PRESS)
  {
    // Move left
    cameraPosition.y += cameraSpeed;
    cameraPosition.y = std::min(cameraPosition.y, 19.0f);
  }

  if (glfwGetKey(this->window, GLFW_KEY_D) == GLFW_PRESS)
  {
    // Move right
    cameraPosition.y -= cameraSpeed;
    cameraPosition.y = std::max(cameraPosition.y, -19.0f);
  }

  if (glfwGetKey(this->window, GLFW_KEY_Q) == GLFW_PRESS)
  {
    // Move up
    cameraPosition.z += cameraSpeed;
    cameraPosition.z = std::min(cameraPosition.z, 19.0f);
  }

  if (glfwGetKey(this->window, GLFW_KEY_E) == GLFW_PRESS)
  {
    // Move down
    cameraPosition.z -= cameraSpeed;
    cameraPosition.z = std::max(cameraPosition.z, 1.0f);
  }

  // Camera direction

  if (glfwGetKey(this->window, GLFW_KEY_1) == GLFW_PRESS)
  {
    // Look forward
    cameraDirection = glm::vec3(1, 0, 0);
  }

  if (glfwGetKey(this->window, GLFW_KEY_2) == GLFW_PRESS)
  {
    // Look backward
    cameraDirection = glm::vec3(-1, 0, 0);
  }

  if (glfwGetKey(this->window, GLFW_KEY_3) == GLFW_PRESS)
  {
    // Look left
    cameraDirection = glm::vec3(0, 1, 0);
  }

  if (glfwGetKey(this->window, GLFW_KEY_4) == GLFW_PRESS)
  {
    // Look right
    cameraDirection = glm::vec3(0, -1, 0);
  }
}

void SingletonApp::execute()
{
  glEnable(GL_DEPTH_TEST);
  glfwSetWindowUserPointer(window, this);
  glfwSetCursorPosCallback(window, SingletonApp::mouseCallback);
  while (!glfwWindowShouldClose(this->window))
  {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    projectionMatrix = glm::perspective(glm::radians(fieldOfView), this->window_resolution.x / this->window_resolution.y, 0.1f, 100.0f);
    viewMatrix = glm::lookAt(cameraPosition, cameraPosition + cameraDirection, cameraUp);
    modelMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f));
    this->conf.enable();
    glUniformMatrix4fv(this->conf.getUniformVarId("uViewMatrix"), 1, GL_FALSE, glm::value_ptr(viewMatrix));
    glUniformMatrix4fv(this->conf.getUniformVarId("uProjectionMatrix"), 1, GL_FALSE, glm::value_ptr(projectionMatrix));
    glUniformMatrix4fv(this->conf.getUniformVarId("uModelMatrix"), 1, GL_FALSE, glm::value_ptr(modelMatrix));
    this->conf.disable();
    this->texture_conf.enable();
    glUniformMatrix4fv(this->texture_conf.getUniformVarId("uViewMatrix"), 1, GL_FALSE, glm::value_ptr(viewMatrix));
    glUniformMatrix4fv(this->texture_conf.getUniformVarId("uProjectionMatrix"), 1, GL_FALSE, glm::value_ptr(projectionMatrix));
    glUniformMatrix4fv(this->texture_conf.getUniformVarId("uModelMatrix"), 1, GL_FALSE, glm::value_ptr(modelMatrix));
    glUniform1i(this->texture_conf.getUniformVarId("uTexture"), 0);
    glBindVertexArray(this->vao);
    // glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->eab);
    // glDrawElements(GL_TRIANGLES, 12, GL_UNSIGNED_SHORT, 0);
    // glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, this->texture_floor);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindTexture(GL_TEXTURE_2D, this->texture_wall);
    glDrawArrays(GL_TRIANGLES, 6, 24);
    glBindTexture(GL_TEXTURE_2D, this->texture_ceiling);
    glDrawArrays(GL_TRIANGLES, 30, 6);
    this->texture_conf.disable();
    for (int i = -10; i <= 10; i = i + 10)
    {
      for (int j = 0; j <= 10; j = j + 10)
      {
        this->texture_conf.enable();
        glActiveTexture(GL_TEXTURE0);
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(0.0f + i, 2.0f + j, 2.0f));
        model = glm::rotate(model, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
        model = glm::scale(model, glm::vec3(1.0f / 25.0f, 1.0f / 10.0f, 1.0f / 20.0f));
        modelMatrix = model;
        glUniformMatrix4fv(this->texture_conf.getUniformVarId("uModelMatrix"), 1, GL_FALSE, glm::value_ptr(modelMatrix));
        glBindTexture(GL_TEXTURE_2D, this->texture_obstacle_bottom);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(0.0f + i, -1.0f + j, 2.0f));
        model = glm::rotate(model, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
        model = glm::scale(model, glm::vec3(1.0f / 25.0f, 1.0f / 10.0f, 1.0f / 20.0f));
        modelMatrix = model;
        glUniformMatrix4fv(this->texture_conf.getUniformVarId("uModelMatrix"), 1, GL_FALSE, glm::value_ptr(modelMatrix));
        glBindTexture(GL_TEXTURE_2D, this->texture_obstacle_bottom);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(0.0f + i, 0.0f + j, 4.0f));
        model = glm::scale(model, glm::vec3(1.0f / 25.0f, 1.0f / 6.0f, 1.0f / 20.0f));
        modelMatrix = model;
        glUniformMatrix4fv(this->texture_conf.getUniformVarId("uModelMatrix"), 1, GL_FALSE, glm::value_ptr(modelMatrix));
        glBindTexture(GL_TEXTURE_2D, this->texture_obstacle_up);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        glBindTexture(GL_TEXTURE_2D, 0);
        glActiveTexture(0);
        this->texture_conf.disable();
      }
    }
    for (int i = -10; i <= 10; i = i + 10)
    {
      for (int j = 0; j <= 3000; j = j + 1)
      {
        this->texture_conf.enable();
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, this->texture_obstacle_bottom);
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(0.0f + i, -10.0f - j * 0.001, 1.0f));
        model = glm::rotate(model, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
        model = glm::scale(model, glm::vec3(5.0f, 5.0f, 5.0f));
        modelMatrix = model;
        glUniformMatrix4fv(this->texture_conf.getUniformVarId("uModelMatrix"), 1, GL_FALSE, glm::value_ptr(modelMatrix));
        glDrawArrays(GL_TRIANGLE_FAN, 36, 52);
        glBindTexture(GL_TEXTURE_2D, 0);
        glActiveTexture(0);
        this->texture_conf.disable();
      }
    }
    this->processInput();
    glfwSwapBuffers(this->window);
    glfwPollEvents();
  }
}

void SingletonApp::prepareTexture(GLuint &texture, std::string filepath, GLint paramWrapS, GLint paramWrapT, GLint paramMinFilter, GLuint paramMagFilter)
{
  /*
        paramWrapS, paramWrapT =
                - GL_CLAMP_TO_EDGE
                - GL_CLAMP_TO_BORDER
                - GL_MIRRORED_REPEAT
                - GL_REPEAT
                - GL_MIRROR_CLAMP_TO_EDGE

        paramMinFilter =
                - GL_NEAREST
                - GL_LINEAR
                - GL_NEAREST_MIPMAP_NEAREST
                - GL_LINEAR_MIPMAP_NEAREST
                - GL_NEAREST_MIPMAP_LINEAR
                - GL_LINEAR_MIPMAP_LINEAR

        paramMagFilter =
                - GL_NEAREST
                - GL_LINEAR

    */

  int imageWidth;
  int imageHeight;
  int imageChannels;

  stbi_set_flip_vertically_on_load(true);

  unsigned char *image = stbi_load(filepath.c_str(), &imageWidth, &imageHeight, &imageChannels, STBI_rgb);

  if (!image)
  {
    std::cerr << "Error loading texture  " << filepath << std::endl;
  }

  glGenTextures(1, &texture);

  glBindTexture(GL_TEXTURE_2D, texture);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, paramWrapS);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, paramWrapT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, paramMinFilter);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, paramMagFilter);

  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, imageWidth, imageHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, image);

  if (paramMinFilter == GL_NEAREST_MIPMAP_NEAREST ||
      paramMinFilter == GL_LINEAR_MIPMAP_NEAREST ||
      paramMinFilter == GL_NEAREST_MIPMAP_LINEAR ||
      paramMinFilter == GL_LINEAR_MIPMAP_LINEAR)
  {
    glGenerateMipmap(GL_TEXTURE_2D);
  }

  glBindTexture(GL_TEXTURE_2D, 0);

  stbi_image_free(image);
}

bool SingletonApp::prepareScene()
{
  bool status;
  status = this->conf.init();
  if (!status)
  {
    return false;
  }
  status = this->texture_conf.init();
  if (!status)
  {
    return false;
  }
  std::vector<Vertex> buffer;
  this->prepareCuboid(buffer);
  this->prepareCircle(buffer);
  this->prepareVbo(buffer, this->vbo);
  this->prepareVao(this->vao);
  // this->prepareEab(this->eab);
  this->prepareTexture(this->texture, "../resources/box.png", GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
  this->prepareTexture(this->texture_wall, "../resources/wall.png", GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
  this->prepareTexture(this->texture_floor, "../resources/floor.png", GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
  this->prepareTexture(this->texture_ceiling, "../resources/ceiling.png", GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
  this->prepareTexture(this->texture_obstacle_bottom, "../resources/obstacle_bottom.png", GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
  this->prepareTexture(this->texture_obstacle_up, "../resources/obstacle_up.png", GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
  return true;
}

void SingletonApp::processMouse(float xoffset, float yoffset)
{
  xoffset *= this->mouse_sensitivity;
  yoffset *= this->mouse_sensitivity;

  this->phi += xoffset;
  this->theta += yoffset;

  glm::vec3 front;
  front.x = sin(glm::radians(this->theta)) * cos(glm::radians(this->phi));
  front.y = cos(glm::radians(this->theta));
  front.z = sin(glm::radians(this->theta)) * sin(glm::radians(this->phi));
  this->cameraDirection = glm::normalize(front);
  // this->right = glm::normalize(glm::cross(this->cameraDirection, this->cameraUp));
  // this->cameraUp = glm::normalize(glm::cross(this->right, this->cameraDirection));
}

void SingletonApp::handleMouseMovement(GLFWwindow *window, double xpos, double ypos)
{
  if (this->first_mouse_movement)
  {
    this->last_x = xpos;
    this->last_y = ypos;
    this->first_mouse_movement = false;
  }

  float xoffset = this->last_x - xpos;
  float yoffset = ypos - this->last_y;
  this->last_x = xpos;
  this->last_y = ypos;

  this->processMouse(xoffset, yoffset);
}

void SingletonApp::mouseCallback(GLFWwindow *window, double ypos, double xpos)
{
  SingletonApp *app = static_cast<SingletonApp *>(glfwGetWindowUserPointer(window));
  if (app)
    app->handleMouseMovement(window, xpos, ypos);
}

SingletonApp::~SingletonApp()
{
  glDeleteVertexArrays(1, &this->vao);
  glDeleteBuffers(1, &this->vbo);
  glDeleteBuffers(1, &this->eab);
  glfwTerminate();
}