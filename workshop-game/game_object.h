

#pragma once

#include "game_context.h"


// GameObject class holds an instance of a b2Body and can process events on it
// (e.g. Collisions). It keeps a pointer to GameContext
class GameObject
{
public:
    GameObject(const GameContext* context);
    virtual ~GameObject();
private:
    const GameContext* game_context;
    b2Body* object;
};
