
#pragma once

#include "game_object.h"

// Bullets fired by the player, go into a straight line in a given direction for
// a few seconds
class Bullet : public PhysicalGameObject
{
public:
    // bullets have a starter position and a direction, they are spawn with a
    // given force and live for a short while.
    Bullet(GameContext* context, b2Vec2 starting, b2Vec2 direction);
    virtual ~Bullet();

    void Update() override;

    GameObjectType GetGameObjectType() override
    {
        // if not overriden, return DEFAULT
        return GameObjectType::BULLET;
    }

private:
    int ttl;
};
