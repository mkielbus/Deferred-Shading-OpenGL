#ifndef GKOM2024L_DATA_HPP
#define GKOM2024L_DATA_HPP

#include <glm/glm.hpp>
#include <iostream>
#include <vector>

struct Vertex
{
    glm::vec3 position;
    glm::vec3 color;

    Vertex()
    {
        this->position = glm::vec3(0.0f, 0.0f, 0.0f);
        this->color = glm::vec3(1.0f, 1.0f, 1.0f);
    }

    Vertex(glm::vec3 pos, glm::vec3 color)
    {
        this->position = pos;
        this->color = color;
    }

    Vertex(const Vertex &other_vertex)
    {
        this->position = other_vertex.position;
        this->color = other_vertex.color;
    }

    Vertex &operator=(const Vertex &other_vertex)
    {
        this->position = other_vertex.position;
        this->color = other_vertex.color;
        return *this;
    }
    ~Vertex() = default;
};

#endif