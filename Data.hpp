#ifndef GKOM2024L_DATA_HPP
#define GKOM2024L_DATA_HPP

#include <glm/glm.hpp>
#include <iostream>
#include <vector>
#include <fstream>

struct Vertex
{
    glm::vec3 position;
    glm::vec3 color;
    glm::vec3 normal;
    glm::vec2 tex2d_coord;

    Vertex()
    {
        this->position = glm::vec3(0.0f, 0.0f, 0.0f);
        this->color = glm::vec3(1.0f, 1.0f, 1.0f);
        this->normal = glm::vec3(0.0f, 1.0f, 0.0f);
        this->tex2d_coord = glm::vec2(0.0f, 0.0f);
    }

    Vertex(glm::vec3 pos, glm::vec3 color, glm::vec3 normal, glm::vec2 tex2d_coord = glm::vec2(0.0f, 0.0f))
    {
        this->position = pos;
        this->color = color;
        this->normal = normal;
        this->tex2d_coord = tex2d_coord;
    }

    Vertex(const Vertex &other_vertex)
    {
        this->position = other_vertex.position;
        this->color = other_vertex.color;
        this->normal = other_vertex.normal;
        this->tex2d_coord = other_vertex.tex2d_coord;
    }

    Vertex &operator=(const Vertex &other_vertex)
    {
        this->position = other_vertex.position;
        this->color = other_vertex.color;
        this->normal = other_vertex.normal;
        this->tex2d_coord = other_vertex.tex2d_coord;
        return *this;
    }
    ~Vertex() = default;
};

#endif