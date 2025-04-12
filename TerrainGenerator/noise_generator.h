#pragma once

#include <glm/glm.hpp>
#include <vector>

class NoiseGenerator {
public:
    NoiseGenerator(unsigned int seed = 0);

    float fractalNoise(glm::vec2 position, int octaves,
        float frequency, float persistence,
        float lacunarity) const;

    float ridgedNoise(glm::vec2 position, float frequency) const;
    float billowNoise(glm::vec2 position, float frequency) const;
    float getBaseNoise(glm::vec2 position, float frequency) const;

private:
    unsigned int seed;
};