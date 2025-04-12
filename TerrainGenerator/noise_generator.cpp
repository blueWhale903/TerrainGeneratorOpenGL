// noise_generator.cpp
#include "noise_generator.h"
#include <glm/gtc/noise.hpp>
#include <cmath>

NoiseGenerator::NoiseGenerator(unsigned int seed) : seed(seed) {}

float NoiseGenerator::getBaseNoise(glm::vec2 position, float frequency) const {
    position *= frequency;
    position += glm::vec2(seed * 100.0f); // Seed affects noise pattern
    return glm::simplex(position) * 0.5f + 0.5f; // Convert [-1,1] to [0,1]
}

float NoiseGenerator::fractalNoise(glm::vec2 position, int octaves,
    float frequency, float persistence,
    float lacunarity) const {
    float total = 0.0f;
    float amplitude = 1.0f;
    float maxAmplitude = 0.0f;

    for (int i = 0; i < octaves; i++) {
        total += getBaseNoise(position, frequency) * amplitude;
        frequency *= lacunarity;
        maxAmplitude += amplitude;
        amplitude *= persistence;
    }

    return total / maxAmplitude;
}

float NoiseGenerator::ridgedNoise(glm::vec2 position, float frequency) const {
    float noise = getBaseNoise(position, frequency);
    noise = 1.0f - std::abs(noise * 2.0f - 1.0f); // Create ridges
    return noise * noise; // Sharpen ridges
}

float NoiseGenerator::billowNoise(glm::vec2 position, float frequency) const {
    float noise = getBaseNoise(position, frequency);
    return std::abs(noise * 2.0f - 1.0f); // Create billowy patterns
}