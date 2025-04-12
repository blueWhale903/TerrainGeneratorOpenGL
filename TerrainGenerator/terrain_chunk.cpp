// terrain_chunk.cpp
#include "terrain_chunk.h"
#include <glm/glm.hpp>
#include <glm/gtc/noise.hpp>
#include <iostream>

TerrainChunk::TerrainChunk(const glm::vec2& position, int size) :
    position(position), size(size),
    frequency(0.05f), amplitude(30.0f),
    octaves(6), persistence(0.5f), lacunarity(2.0f) {
    generateMesh();
}

TerrainChunk::~TerrainChunk() {
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
}

void TerrainChunk::generateMesh() {
    const int resolution = size;
    const float step = 1.0f;

    // Generate vertices
    for (int z = 0; z <= resolution; ++z) {
        for (int x = 0; x <= resolution; ++x) {
            float worldX = position.x + x * step;
            float worldZ = position.y + z * step;
            float height = calculateHeight(worldX, worldZ);

            // Position
            vertices.push_back(worldX);
            vertices.push_back(height);
            vertices.push_back(worldZ);

            // Normal (will be calculated later)
            vertices.push_back(0.0f);
            vertices.push_back(1.0f);
            vertices.push_back(0.0f);

            // Texture coordinates
            vertices.push_back((float)x / resolution);
            vertices.push_back((float)z / resolution);
        }
    }

    // Generate indices
    for (int z = 0; z < resolution; ++z) {
        for (int x = 0; x < resolution; ++x) {
            int topLeft = z * (resolution + 1) + x;
            int topRight = topLeft + 1;
            int bottomLeft = (z + 1) * (resolution + 1) + x;
            int bottomRight = bottomLeft + 1;

            indices.push_back(topLeft);
            indices.push_back(bottomLeft);
            indices.push_back(topRight);

            indices.push_back(topRight);
            indices.push_back(bottomLeft);
            indices.push_back(bottomRight);
        }
    }

    // Calculate normals
    for (size_t i = 0; i < indices.size(); i += 3) {
        unsigned int i0 = indices[i] * 8;
        unsigned int i1 = indices[i + 1] * 8;
        unsigned int i2 = indices[i + 2] * 8;

        glm::vec3 v0(vertices[i0], vertices[i0 + 1], vertices[i0 + 2]);
        glm::vec3 v1(vertices[i1], vertices[i1 + 1], vertices[i1 + 2]);
        glm::vec3 v2(vertices[i2], vertices[i2 + 1], vertices[i2 + 2]);

        glm::vec3 edge1 = v1 - v0;
        glm::vec3 edge2 = v2 - v0;
        glm::vec3 normal = glm::normalize(glm::cross(edge1, edge2));

        // Add the normal to each vertex of the triangle
        for (int j = 0; j < 3; ++j) {
            unsigned int idx = indices[i + j] * 8 + 3;
            vertices[idx] += normal.x;
            vertices[idx + 1] += normal.y;
            vertices[idx + 2] += normal.z;
        }
    }

    // Normalize all normals
    for (size_t i = 3; i < vertices.size(); i += 8) {
        glm::vec3 normal(vertices[i], vertices[i + 1], vertices[i + 2]);
        normal = glm::normalize(normal);
        vertices[i] = normal.x;
        vertices[i + 1] = normal.y;
        vertices[i + 2] = normal.z;
    }

    // Setup OpenGL buffers
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);

    // Position attribute
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);

    // Normal attribute
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));

    // Texture coordinate attribute
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));

    glBindVertexArray(0);
}

// In terrain_chunk.cpp
float TerrainChunk::calculateHeight(float x, float z) const {
    glm::vec2 position(x, z);

    // Base terrain shape (large features)
    float baseHeight = noiseGenerator.fractalNoise(
        position * 0.5f, 8, 0.005f, 0.5f, 2.0f) * 100.0f;

    // Medium details
    float mediumDetails = noiseGenerator.fractalNoise(
        position, 5, 0.02f, 0.5f, 2.2f) * 30.0f;

    // Fine details
    float fineDetails = noiseGenerator.fractalNoise(
        position * 2.0f, 3, 0.1f, 0.5f, 2.5f) * 10.0f;

    // Ridges for mountain ranges
    float ridges = noiseGenerator.ridgedNoise(position * 0.3f, 0.01f) * 50.0f;

    // Combine all layers
    float height = baseHeight * 0.7f + mediumDetails * 0.2f +
        fineDetails * 0.1f + ridges * 0.3f;

    // Flatten low areas
    if (height < 15.0f) {
        height = height * 0.3f + 10.5f;
    }

    return height;
}

void TerrainChunk::render(const std::shared_ptr<Shader>& shader) {
    glm::mat4 model = glm::mat4(1.0f);
    shader->setMat4("model", model);

    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, static_cast<unsigned int>(indices.size()), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}