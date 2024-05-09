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

struct Triangle
{
    Vertex vertexes[3];
    Triangle()
    {
        unsigned int iterator = 0;
        for (iterator; iterator < 3; iterator++)
        {
            this->vertexes[iterator] = Vertex();
        }
    }
    Triangle(std::vector<Vertex> vertexes)
    {
        unsigned int iterator = 0;
        for (const Vertex &other_vertex : vertexes)
        {
            this->vertexes[iterator] = other_vertex;
            iterator++;
        }
    }
    ~Triangle() = default;
};

#endif