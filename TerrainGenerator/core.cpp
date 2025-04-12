#include "core.h"
#include "camera.h"
#include "shader.h"
#include "terrain_chunk.h"
#include <iostream>

CoreSystem& CoreSystem::getInstance() {
    static CoreSystem instance;
    return instance;
}

CoreSystem::CoreSystem() : window(nullptr), windowWidth(0), windowHeight(0), wireframeMode(false) {}

CoreSystem::~CoreSystem() {}

bool CoreSystem::initialize(int width, int height, const char* title) {
    windowWidth = width;
    windowHeight = height;

    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        return false;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    window = glfwCreateWindow(width, height, title, nullptr, nullptr);
    if (!window) {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return false;
    }

    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cerr << "Failed to initialize GLAD" << std::endl;
        return false;
    }

    setupOpenGL();

    camera = std::make_shared<Camera>(glm::vec3(0.0f, 50.0f, 0.0f));
    terrainShader = std::make_shared<Shader>("shaders/terrain_vertex.glsl", "shaders/terrain_fragment.glsl");

    generateTerrain();

    return true;
}

void CoreSystem::setupOpenGL() {
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glFrontFace(GL_CCW);
    glClearColor(0.53f, 0.81f, 0.92f, 1.0f);
}

void CoreSystem::run() {
    float lastFrame = 0.0f;

    while (!glfwWindowShouldClose(window)) {
        float currentFrame = glfwGetTime();
        float deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        processInput(deltaTime);
        update(deltaTime);
        render();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
}

void CoreSystem::processInput(float deltaTime) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera->processKeyboard(Camera::FORWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera->processKeyboard(Camera::BACKWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera->processKeyboard(Camera::LEFT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera->processKeyboard(Camera::RIGHT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
        camera->processKeyboard(Camera::UP, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
        camera->processKeyboard(Camera::DOWN, deltaTime);

    if (glfwGetKey(window, GLFW_KEY_F1) == GLFW_PRESS) {
        wireframeMode = !wireframeMode;
        glPolygonMode(GL_FRONT_AND_BACK, wireframeMode ? GL_LINE : GL_FILL);
    }
}

void CoreSystem::update(float deltaTime) {
    // Update camera
    double xpos, ypos;
    glfwGetCursorPos(window, &xpos, &ypos);
    camera->processMouseMovement(xpos, ypos);

    // Update terrain chunks (LOD, loading/unloading)
    // TODO: Implement dynamic terrain chunk loading based on camera position
}

void CoreSystem::render() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glm::mat4 projection = glm::perspective(glm::radians(camera->getZoom()),
        (float)windowWidth / (float)windowHeight,
        0.1f, 1000.0f);
    glm::mat4 view = camera->getViewMatrix();

    terrainShader->use();
    terrainShader->setMat4("projection", projection);
    terrainShader->setMat4("view", view);
    terrainShader->setVec3("lightPos", glm::vec3(0,50,0));
    terrainShader->setVec3("viewPos", camera->getPosition());
    terrainShader->setVec3("lightColor", glm::vec3(1.25f));

    for (auto& chunk : terrainChunks) {
        chunk->render(terrainShader);
    }
}

void CoreSystem::generateTerrain() {
    // Generate initial terrain chunks around the origin
    const int chunkSize = 64;
    const int renderDistance = 3; // chunks in each direction

    for (int x = -renderDistance; x <= renderDistance; ++x) {
        for (int z = -renderDistance; z <= renderDistance; ++z) {
            glm::vec2 position(x * chunkSize, z * chunkSize);
            terrainChunks.push_back(std::make_shared<TerrainChunk>(position, chunkSize));
        }
    }
}

void CoreSystem::shutdown() {
    terrainShader.reset();
    camera.reset();
    terrainChunks.clear();

    if (window) {
        glfwDestroyWindow(window);
        window = nullptr;
    }
    glfwTerminate();
}