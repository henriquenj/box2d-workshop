
#pragma once

#include "game_object.h"


// Character class process chracter movement and fire bullets

class Character : public PhysicalGameObject
{
public:
    Character(GameContext* context);

    virtual void Update() override;
    virtual void OnKeyPress(int key, int scancode, int action, int mods) override;
    virtual void OnMousePress(int32 button, int32 action, int32 mods) override;
    virtual ~Character();

    b2Vec2 GetPosition() const;

private:
    // Enum to represent the current movement of the character (which key is
    // being held down, basically)
    enum CharMovement {RIGHT, LEFT, STOP};
    CharMovement movement;
};