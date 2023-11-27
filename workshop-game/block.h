

#pragma once

#include "game_object.h"


// The object of the game is to hit the blocks with bullets
class Block : public PhysicalGameObject
{
public:
    // The block object receives the starter_center parameter at construction,
    // which is relative to the Character. The Block will then be instantiated
    // around this center.
    Block(GameContext* context, float starter_center);
    virtual ~Block();

    void Update() override;

private:
    // size of the bloc, randomly generated
    int size;
    // time to live of this block, it is deleted when it reaches zero
    int time_to_live;
};
