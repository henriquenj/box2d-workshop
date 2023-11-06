
#pragma once

#include <memory>
#include <vector>

#define GLFW_INCLUDE_NONE
#include "GLFW/glfw3.h"
#include "box2d/box2d.h"
#include <text.h>


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
    // All text objects in the game. Rendered once per frame.
    std::vector<Text> all_text;
};
