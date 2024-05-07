#include <iostream>
#include <fstream>
#include <vector>

// Include GLEW - niezbędne do zarządzania rozszerzeniami OpenGL
#define GLEW_STATIC
#include <GL/glew.h>

// Include GLFW - biblioteka do obsługi okien
#include <GLFW/glfw3.h>

// Dane trójkątów (pozycje wierzchołków i kolory)
GLfloat vertices[] = {
    // Trójkąt niebieski
    -0.99f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f,
    -0.7f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f,
    -0.7f, 0.5f, 0.0f, 0.0f, 0.0f, 1.0f,
    // Trójkąt czerwony
    0.99f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
    0.7f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f,
    0.7f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f,
    // Trójkąt zielony
    -0.7f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f,
    0.7f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f,
    0.0f, 0.8f, 0.0f, 0.0f, 1.0f, 0.0f};

int main()
{
    // Inicjalizacja GLFW
    if (!glfwInit())
    {
        std::cerr << "Błąd inicjalizacji GLFW" << std::endl;
        return -1;
    }

    // Konfiguracja GLFW
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);

    // Utwórz okno
    GLFWwindow *window = glfwCreateWindow(800, 600, "Scena 3D", nullptr, nullptr);
    if (!window)
    {
        std::cerr << "Błąd tworzenia okna GLFW" << std::endl;
        glfwTerminate();
        return -1;
    }

    // Ustaw kontekst OpenGL dla okna
    glfwMakeContextCurrent(window);

    // Inicjalizacja GLEW
    if (glewInit() != GLEW_OK)
    {
        std::cerr << "Błąd inicjalizacji GLEW" << std::endl;
        glfwTerminate();
        return -1;
    }

    // Utwórz i skompiluj vertex shader
    const char *vertexShaderSource =
        "#version 130\n"
        "in vec3 position;\n"
        "in vec3 color;\n"
        "out vec3 fragColor;\n"
        "void main() {\n"
        "   gl_Position = vec4(position, 1.0);\n"
        "   fragColor = color;\n"
        "}\n";

    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, nullptr);
    glCompileShader(vertexShader);

    // Utwórz i skompiluj fragment shader
    const char *fragmentShaderSource =
        "#version 130\n"
        "in vec3 fragColor;\n"
        "out vec4 finalColor;\n"
        "void main() {\n"
        "   finalColor = vec4(fragColor, 1.0);\n"
        "}\n";

    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, nullptr);
    glCompileShader(fragmentShader);

    // Utwórz program shaderowy i dołącz skompilowane shadery
    GLuint shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    glUseProgram(shaderProgram);

    // Utwórz i skonfiguruj VAO (Vertex Array Object) i VBO (Vertex Buffer Object)
    GLuint VAO, VBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // Atrybut pozycji wierzchołków
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid *)0);
    glEnableVertexAttribArray(0);

    // Atrybut koloru wierzchołków
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid *)(3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    // Główna pętla programu
    while (!glfwWindowShouldClose(window))
    {
        // Czyszczenie bufora koloru
        glClear(GL_COLOR_BUFFER_BIT);

        // Narysuj trójkąty
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 9);
        glBindVertexArray(0);

        // Sprawdź zdarzenia i zamień bufory
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // Zwolnij zasoby
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    return 0;
}