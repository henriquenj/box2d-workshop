

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
protected:
    b2Body* body;
    const GameContext* game_context;
};
