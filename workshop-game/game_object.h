

#pragma once

#include "game_context.h"
#include "box2d/box2d.h"


// GameObject class holds an instance of a b2Body and can process events on it
// (e.g. Collisions). It keeps a pointer to GameContext
class GameObject
{
public:
    GameObject(const GameContext* context);
    virtual ~GameObject();

    // Called every frame
    virtual void Update();
protected:
    const GameContext* game_context;
};


// A GameObject that also contains a b2Body
class PhysicalGameObject : public GameObject
{

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
};
