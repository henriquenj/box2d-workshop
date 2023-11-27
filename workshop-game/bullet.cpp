

#include "bullet.h"

Bullet::Bullet(GameContext* context, b2Vec2 starting, b2Vec2 direction)
    : PhysicalGameObject(context)
{
    // lives for one second
    ttl = 60;

    b2CircleShape shape;
    shape.m_radius = 0.4f;
    b2FixtureDef fd;
    // set a group for the Bullet, so it won't collide with the character
    fd.filter.groupIndex = -1;
    fd.shape = &shape;
    fd.density = 0.1f;
    fd.friction = 0.1f;
    b2BodyDef bd;
    bd.type = b2_dynamicBody;
    bd.position = starting;
    // set userData
    bd.userData.pointer = (uintptr_t)this;
    body = context->world->CreateBody(&bd);
    body->CreateFixture(&fd);
    // treat this body as bullet for collision detection purposes
    body->SetBullet(true);

    // apply bullet impulse, so they fly into the given direction
    body->ApplyLinearImpulseToCenter(direction, true);

}

Bullet::~Bullet()
{
}

void Bullet::Update()
{
    ttl--;
    // when ttl reaches 0, destroy this bullet
    if (ttl <= 0)
    {
        shouldDelete = true;
    }
}
