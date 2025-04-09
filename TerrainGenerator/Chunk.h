#pragma once

#include <glm/glm.hpp>
#include "Terrain.h"

class Chunk {
public:
    static const int CHUNK_SIZE = 32;     static constexpr float VERTEX_SPACING = 1.0f;
    static constexpr float WORLD_SIZE = CHUNK_SIZE * VERTEX_SPACING;

    Chunk(int gridX, int gridZ);
    void Render();
    glm::ivec2 GetPosition() const { return glm::ivec2(gridX, gridZ); }

private:
    int gridX, gridZ;     Terrain terrain;
    bool initialized = false;
};