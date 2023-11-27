
#pragma once

#include <memory>
#include <vector>

#define GLFW_INCLUDE_NONE
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
    // player start with 10 lives
    int lives = 10;
    // the level of the game, changes every time lives reaches 20
    int level = 1;

    // All game objects in the game. Can be any type of game object
    std::vector<std::unique_ptr<GameObject>> all_objects;
    // objects created during the Update() function should be created here
    std::vector<std::unique_ptr<GameObject>> to_create;

    // Get mouse position with those getters

    // Get mouse position in Window coordinates
    b2Vec2 GetMousePosWindow() const
    {
        return mouse_position_window;
    }

    // Get mouse position in Box2D coordinates
    b2Vec2 GetMousePosWorld() const
    {
        return mouse_position_box2d;
    }

    // for updating mouse positions, only this function is allowed to do that
    friend void MouseMotionCallback(GLFWwindow*, double xd, double yd);

private:
    // the current position of the mouse in window coordinates
    b2Vec2 mouse_position_window;
    // the current position of the mouse in Box2D coordinates
    b2Vec2 mouse_position_box2d;
};
