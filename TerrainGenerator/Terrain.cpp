#include "Terrain.h"
#include "SimpleNoise.h"
#include "Chunk.h"
#include <vector>

Terrain::Terrain(int gridX, int gridZ, int width, int depth, float scale)
    : gridX(gridX), gridZ(gridZ), width(width), depth(depth), scale(scale) {
    GenerateTerrain();
}

float Terrain::GenerateHeight(int x, int z) const {
    PerlinNoise pn;

        float worldX = (gridX * (width - 1) + x) * Chunk::VERTEX_SPACING;
    float worldZ = (gridZ * (depth - 1) + z) * Chunk::VERTEX_SPACING;

    float height = 0;
    float amplitude = 1.0f;
    float frequency = 0.01f;  
        for (int i = 0; i < 5; i++) {
        float sampleX = worldX * frequency;
        float sampleZ = worldZ * frequency;

                float noiseValue = pn.noise(sampleX, sampleZ, 0.5) * 2.0f - 1.0f;

        height += noiseValue * amplitude;
        amplitude *= 0.5f;            frequency *= 2.0f;        }

        return (height + 1.0f) * 0.5f * scale;  }

void Terrain::GenerateTerrain() {
    PerlinNoise pn;

        for (int z = 0; z < depth; ++z) {
        for (int x = 0; x < width; ++x) {
                        float height = 0;
            float amplitude = 1.0f;
            float frequency = 1.0f;

            for (int i = 0; i < 4; ++i) {                 float sampleX = x / (float)width * frequency;
                float sampleZ = z / (float)depth * frequency;

                height += pn.noise(sampleX, sampleZ, 0.5) * amplitude;

                amplitude *= 0.5f;                 frequency *= 2.0f;             }

            height *= scale;

                        vertices.push_back(x - width / 2.0f);             vertices.push_back(height);                     vertices.push_back(z - depth / 2.0f); 
                        glm::vec3 color;
            if (height < 0) {
                color = glm::vec3(0.0f, 0.0f, 0.7f);             }
            else if (height < 5) {
                color = glm::vec3(0.0f, 0.0f, 1.0f);             }
            else if (height < 10) {
                color = glm::vec3(0.9f, 0.9f, 0.5f);             }
            else if (height < 20) {
                color = glm::vec3(0.0f, 0.6f, 0.0f);             }
            else if (height < 30) {
                color = glm::vec3(0.5f, 0.5f, 0.5f);             }
            else {
                color = glm::vec3(1.0f, 1.0f, 1.0f);             }

            vertices.push_back(color.r);
            vertices.push_back(color.g);
            vertices.push_back(color.b);
        }
    }

        for (int z = 0; z < depth - 1; ++z) {
        for (int x = 0; x < width - 1; ++x) {
            int topLeft = (z * width) + x;
            int topRight = topLeft + 1;
            int bottomLeft = ((z + 1) * width) + x;
            int bottomRight = bottomLeft + 1;

                        indices.push_back(topLeft);
            indices.push_back(bottomLeft);
            indices.push_back(topRight);

                        indices.push_back(topRight);
            indices.push_back(bottomLeft);
            indices.push_back(bottomRight);
        }
    }

    indexCount = indices.size();

        normals.resize(vertices.size(), 0.0f);
    for (size_t i = 0; i < indices.size(); i += 3) {
        unsigned int i0 = indices[i];
        unsigned int i1 = indices[i + 1];
        unsigned int i2 = indices[i + 2];

        glm::vec3 v0(vertices[i0 * 6], vertices[i0 * 6 + 1], vertices[i0 * 6 + 2]);
        glm::vec3 v1(vertices[i1 * 6], vertices[i1 * 6 + 1], vertices[i1 * 6 + 2]);
        glm::vec3 v2(vertices[i2 * 6], vertices[i2 * 6 + 1], vertices[i2 * 6 + 2]);

        glm::vec3 normal = glm::normalize(glm::cross(v1 - v0, v2 - v0));

                for (int j = 0; j < 3; ++j) {
            normals[i0 * 3 + j] += normal[j];
            normals[i1 * 3 + j] += normal[j];
            normals[i2 * 3 + j] += normal[j];
        }
    }

        for (size_t i = 0; i < normals.size(); i += 3) {
        glm::vec3 normal(normals[i], normals[i + 1], normals[i + 2]);
        normal = glm::normalize(normal);
        normals[i] = normal.x;
        normals[i + 1] = normal.y;
        normals[i + 2] = normal.z;
    }

        glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

        glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), &vertices[0], GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

        GLuint normalVBO;
    glGenBuffers(1, &normalVBO);
    glBindBuffer(GL_ARRAY_BUFFER, normalVBO);
    glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(float), &normals[0], GL_STATIC_DRAW);

    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(2);

    glBindVertexArray(0);
}

void Terrain::Render() {
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}