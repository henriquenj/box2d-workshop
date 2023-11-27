

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

    virtual GameObjectType GetGameObjectType() override
    {
        return GameObjectType::BLOCK;
    }

    void Update() override;
    void OnCollision(PhysicalGameObject* other, b2Contact* contact) override;

private:
    // size of the bloc, randomly generated
    int size;
    // time to live of this block, it is deleted when it reaches zero
    int time_to_live;
    // the amount of time (frame) "to live" when the blocks are dying
    int time_to_live_dying;

    // the blocks have two states, where they are "dying" or "alive". Blocks
    // remain on the screen for a few frames before despawning when they are
    // dying
    enum BlockState {ALIVE, DYING};
    BlockState state;
};
