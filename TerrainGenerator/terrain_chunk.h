#include <glad/glad.h>
#include <glm/glm.hpp>
#include <vector>
#include <memory>
#include "shader.h"
#include "noise_generator.h"

class TerrainChunk {
public:
    TerrainChunk(const glm::vec2& position, int size);
    ~TerrainChunk();

    void render(const std::shared_ptr<Shader>& shader);

    const glm::vec2& getPosition() const { return position; }
    int getSize() const { return size; }

private:
    void generateMesh();
    float calculateHeight(float x, float z) const;

    glm::vec2 position;
    int size;

    unsigned int VAO, VBO, EBO;
    std::vector<float> vertices;
    std::vector<unsigned int> indices;

    // Noise parameters
    NoiseGenerator noiseGenerator;
    float frequency;
    float amplitude;
    int octaves;
    float persistence;
    float lacunarity;
};