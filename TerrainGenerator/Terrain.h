#pragma once

#include "SimpleNoise.h"
#include <GL/glew.h>
#include <glm/glm.hpp>

class Terrain {
public:
    Terrain(int gridX, int gridZ, int width, int depth, float scale);
    void Render();

private:
    void GenerateTerrain();

    int gridX, gridZ;     int width, depth;
    float scale;

        GLuint VAO, VBO, EBO;
    size_t indexCount;

    std::vector<float> vertices;
    std::vector<unsigned int> indices;
    std::vector<float> normals;

        float GenerateHeight(int x, int z) const;
};