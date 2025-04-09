#include "Chunk.h"

Chunk::Chunk(int gridX, int gridZ) : gridX(gridX), gridZ(gridZ),
terrain(gridX, gridZ, CHUNK_SIZE, CHUNK_SIZE, 20.0f) {}

void Chunk::Render() {
	terrain.Render();

	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	terrain.Render();
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}