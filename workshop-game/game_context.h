
#pragma once

#include "GLFW/glfw3.h"
#include "box2d/box2d.h"

// Game context class holds the game world and some other main entities for the
// game.

class GameContext
{
public:

    // GLFW main window pointer
    GLFWwindow* mainWindow = nullptr;
    // Box2D game world
    b2World* world = nullptr;
};
