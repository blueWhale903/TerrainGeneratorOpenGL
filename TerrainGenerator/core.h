#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <vector>
#include <memory>

class TerrainChunk;
class Camera;
class Shader;

class CoreSystem {
public:
    static CoreSystem& getInstance();

    bool initialize(int width, int height, const char* title);
    void run();
    void shutdown();

    GLFWwindow* getWindow() const { return window; }
    std::shared_ptr<Camera> getCamera() const { return camera; }

private:
    CoreSystem();
    ~CoreSystem();

    void processInput(float deltaTime);
    void update(float deltaTime);
    void render();
    void setupOpenGL();
    void generateTerrain();

    GLFWwindow* window;
    std::shared_ptr<Camera> camera;
    std::shared_ptr<Shader> terrainShader;
    std::vector<std::shared_ptr<TerrainChunk>> terrainChunks;

    int windowWidth, windowHeight;
    bool wireframeMode;
};