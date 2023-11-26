

#pragma once

#include "game_context.h"
#include "box2d/box2d.h"


// GameObject class holds an instance of a b2Body and can process events on it
// (e.g. Collisions). It keeps a pointer to GameContext
class GameObject
{
public:
    GameObject();
    virtual ~GameObject();

    // Called every frame
    virtual void Update();

    // Called every time there's a keyboard press.
    // code for keys here https://www.glfw.org/docs/3.3/group__keys.html
    // and modifiers https://www.glfw.org/docs/3.3/group__mods.html
    virtual void OnKeyPress(int key, int scancode, int action, int mods);

    // Called everytime there's a mouse press
    // code for mouse button keys https://www.glfw.org/docs/3.3/group__buttons.html
    // and modifiers https://www.glfw.org/docs/3.3/group__buttons.html
    // action is either GLFW_PRESS or GLFW_RELEASE
    virtual void OnMousePress(int32 button, int32 action, int32 mods);

    // Signals the engine if object should be deleted
    bool ShouldDelete() const
    {
        return shouldDelete;
    }

protected:
    bool shouldDelete = false;
};


// A GameObject that also contains a b2Body
class PhysicalGameObject : public GameObject
{
public:

    // Main constructor
    PhysicalGameObject(const GameContext* context);

    // Copy constructor and copy assigment are deleted for now.
    // TODO: implement those
    PhysicalGameObject(const PhysicalGameObject& other) = delete;
    PhysicalGameObject& operator=(const PhysicalGameObject& other) = delete;

    // Move constructor
    PhysicalGameObject(PhysicalGameObject&& other);

    // Move assigment operator
    PhysicalGameObject& operator=(PhysicalGameObject&& other);

    virtual ~PhysicalGameObject();

    // Called when this b2Body collides with another b2Body
    virtual void OnCollision(PhysicalGameObject* other);

protected:
    b2Body* body;
    const GameContext* game_context;
};
