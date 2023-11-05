
#pragma once

#include <memory>
#include <vector>

#include "GLFW/glfw3.h"
#include "box2d/box2d.h"


class GameObject;

// Game context class holds the game world and some other main entities for the
// game.

class GameContext
{
public:

    // GLFW main window pointer
    GLFWwindow* mainWindow = nullptr;
    // Box2D game world
    b2World* world = nullptr;

    // All game objects in the game. Can be any type of game object
    std::vector<std::unique_ptr<GameObject>> all_objects;
};
