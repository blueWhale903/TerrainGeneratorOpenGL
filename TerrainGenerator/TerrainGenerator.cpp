// main.cpp
#include "core.h"

int main() {
    CoreSystem& core = CoreSystem::getInstance();

    if (!core.initialize(1280, 720, "Infinite Terrain Generator")) {
        return -1;
    }

    core.run();
    core.shutdown();

    return 0;   
}