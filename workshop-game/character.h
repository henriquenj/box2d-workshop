
#pragma once

#include "game_object.h"


// Character class process chracter movement and fire bullets

class Character : public PhysicalGameObject
{
public:
    Character(const GameContext* context);

    virtual void Update() override;
    virtual void OnKeyPress(int key, int scancode, int action, int mods) override;
    virtual ~Character();

private:
    // Enum to represent the current movement of the character (which key is
    // being held down, basically)
    enum CharMovement {RIGHT, LEFT, STOP};
    CharMovement movement;
};
