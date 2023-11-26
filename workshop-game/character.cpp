
#include <iostream>
#include <cmath>

#include "character.h"
#include "draw_game.h"

Character::Character(const GameContext* context)
    : PhysicalGameObject(context)
{
    b2CircleShape circle_shape;
    circle_shape.m_radius = 1.0f;
    b2FixtureDef circle_fd;
    circle_fd.shape = &circle_shape;
    circle_fd.density = 20.0f;
    circle_fd.friction = 1.0f;
    b2BodyDef char_bd;
    char_bd.type = b2_dynamicBody;
    char_bd.position.Set(0.0f, 1.004f);
    body = context->world->CreateBody(&char_bd);
    body->CreateFixture(&circle_fd);
    body->SetAngularVelocity(2.0f);

    movement = STOP;
}

Character::~Character()
{
}

void Character::Update()
{
    b2Vec2 pos = body->GetPosition();
    // center camera on the character
    g_camera.m_center.x = pos.x;
    g_camera.m_center.y = pos.y + 20;

    // point the body to the mouse
    const b2Vec2 mouse_position = game_context->GetMousePosWorld();
    const b2Vec2 char_position = body->GetWorldCenter();

    // roughly based on this answer https://stackoverflow.com/a/2676810
    float delta_x = mouse_position.x - char_position.x;
    float delta_y = mouse_position.y - char_position.y;
    float angle = atan2(delta_y, delta_x);
    // update body with the new angle
    body->SetTransform(body->GetPosition(), angle);


    // process movement to left and right here
    b2Vec2 vel = body->GetLinearVelocity();
    float velChange, impulse, speed;

    speed = 10;
    switch (movement)
    {
        case Character::RIGHT:
            // go to right
            velChange = speed - vel.x;
            impulse = body->GetMass() * velChange;
            body->ApplyLinearImpulse(b2Vec2(impulse, 0), body->GetWorldCenter(), true);
            break;
        case Character::LEFT:
            // go to left
            velChange = -(speed) - vel.x;
            impulse = body->GetMass() * velChange;
            body->ApplyLinearImpulse(b2Vec2(impulse, 0), body->GetWorldCenter(), true);
            break;
        case Character::STOP:
            // stop character
            velChange = 0 - vel.x;
            impulse = body->GetMass() * velChange;
            body->ApplyLinearImpulse(b2Vec2(impulse, 0), body->GetWorldCenter(), true);
            break;
    }

}

void Character::OnKeyPress(int key, int scancode, int action, int mods)
{
    if (action == GLFW_PRESS)
    {
        switch (key)
        {
            // set the movement status
            case GLFW_KEY_D:
                movement = Character::RIGHT;
                break;
            case GLFW_KEY_A:
                movement = Character::LEFT;
                break;
        }
    }
    // make body stop if one of the keys is released
    else if (action == GLFW_RELEASE)
    {
        switch (key)
        {
            case GLFW_KEY_D:
            case GLFW_KEY_A:
                movement = Character::STOP;
                break;
        }
    }

}

b2Vec2 Character::GetPosition() const
{
    return body->GetPosition();
}
