#include "ChunkManager.h"
#include <algorithm>
#include <iostream>

ChunkManager::ChunkManager(int renderDistance) 
    : renderDistance(renderDistance) {
    lastChunkX = 0;
    lastChunkZ = 0;
}

std::string ChunkManager::GetChunkKey(int x, int z) const {
    return std::to_string(x) + "," + std::to_string(z);
}

void ChunkManager::Update(const glm::vec3& playerPos) {
        const float chunkWorldSize = Chunk::CHUNK_SIZE * Chunk::VERTEX_SPACING;
    int currentChunkX = static_cast<int>(std::floor(playerPos.x / chunkWorldSize));
    int currentChunkZ = static_cast<int>(std::floor(playerPos.z / chunkWorldSize));
    
        if (currentChunkX != lastChunkX || currentChunkZ != lastChunkZ) {
        std::cout << "Player moved to chunk (" << currentChunkX << ", " << currentChunkZ << ")\n";
        
                for (int x = currentChunkX - renderDistance; x <= currentChunkX + renderDistance; x++) {
            for (int z = currentChunkZ - renderDistance; z <= currentChunkZ + renderDistance; z++) {
                LoadChunk(x, z);
            }
        }
        
                UnloadDistantChunks(currentChunkX, currentChunkZ);
        
        lastChunkX = currentChunkX;
        lastChunkZ = currentChunkZ;
    }
}

void ChunkManager::LoadChunk(int x, int z) {
    std::string key = GetChunkKey(x, z);
    
    if (chunks.find(key) == chunks.end()) {
        chunks.emplace(key, Chunk(x, z));
        std::cout << "Loaded chunk at (" << x << ", " << z << ")\n";
    }
}

void ChunkManager::UnloadDistantChunks(int currentChunkX, int currentChunkZ) {
    std::vector<std::string> toRemove;
    
    for (const auto& pair : chunks) {
        const Chunk& chunk = pair.second;
        glm::ivec2 chunkPos = chunk.GetPosition();
        
        int dx = std::abs(chunkPos.x - currentChunkX);
        int dz = std::abs(chunkPos.y - currentChunkZ);
        
        if (dx > renderDistance || dz > renderDistance) {
            toRemove.push_back(pair.first);
        }
    }
    
    for (const auto& key : toRemove) {
        chunks.erase(key);
        std::cout << "Unloaded chunk " << key << "\n";
    }
}

void ChunkManager::Render() {
    for (auto& pair : chunks) {
        pair.second.Render();
    }
}