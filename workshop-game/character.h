
#pragma once

#include "game_object.h"


// Character class process chracter movement and fire bullets

class Character : public PhysicalGameObject
{
public:
    Character(const GameContext* context);
    virtual ~Character();
};
