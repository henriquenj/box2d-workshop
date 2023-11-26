

#pragma once

#include "game_object.h"


// The object of the game is to hit the blocks with bullets
class Block : public PhysicalGameObject
{
public:
    // The block object receives the starter_x parameter at construction, which
    // is relative to the Character
    Block(const GameContext* context, float starter_x);
    virtual ~Block();

private:
    // size of the bloc, randomly generated
    int size;
};
