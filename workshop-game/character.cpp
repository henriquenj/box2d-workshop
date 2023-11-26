

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
    char_bd.position.Set(0, 5);
    body = context->world->CreateBody(&char_bd);
    body->CreateFixture(&circle_fd);
    body->SetAngularVelocity(2.0f);
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
}
