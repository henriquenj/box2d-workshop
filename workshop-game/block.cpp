
#include <random>

#include "block.h"


Block::Block(const GameContext* context, float starter_x)
    : PhysicalGameObject(context)
{

    // size of the blocks are random
    std::random_device dev;
    std::mt19937 rng(dev());
    // distribution in range [1, 6]
    std::uniform_int_distribution<std::mt19937::result_type> dist100(1, 6);

    b2PolygonShape shape;
    shape.SetAsBox(dist100(rng) / 2.5f, dist100(rng) / 2.5f);
    b2FixtureDef fd;
    fd.shape = &shape;
    fd.density = 20.0f;
    fd.friction = 0.1f;
    b2BodyDef bd;
    bd.type = b2_dynamicBody;
    bd.position.Set(starter_x, 30.0f);
    body = context->world->CreateBody(&bd);
    body->CreateFixture(&fd);
}

Block::~Block()
{
}
