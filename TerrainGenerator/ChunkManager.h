#pragma once

#include <vector>
#include <unordered_map>
#include <string>

#include "Chunk.h"

class ChunkManager {
public:
    ChunkManager(int renderDistance = 8);
    void Update(const glm::vec3& playerPos);
    void Render();

private:
    int renderDistance;
    int lastChunkX;
    int lastChunkZ;
    std::unordered_map<std::string, Chunk> chunks;

    std::string GetChunkKey(int x, int z) const;
    void LoadChunk(int x, int z);
    void UnloadDistantChunks(int currentChunkX, int currentChunkZ);
};

