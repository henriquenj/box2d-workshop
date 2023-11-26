
#include <random>

#include "block.h"


Block::Block(const GameContext* context, float starter_center)
    : PhysicalGameObject(context)
{

    // size of the blocks are random
    std::random_device dev;
    std::mt19937 rng(dev());
    // distribution in range [1, 3]
    // std::uniform_int_distribution<std::mt19937::result_type> dist3(1, 3);
    // distribution in range [0, 30]
    std::uniform_int_distribution<std::mt19937::result_type> dist30(0, 30);

    b2PolygonShape shape;
    // shape.SetAsBox(dist3(rng) / 2.5f, dist3(rng) / 2.5f);
    shape.SetAsBox(1.5f, 0.5f);
    b2FixtureDef fd;
    fd.shape = &shape;
    fd.density = 20.0f;
    fd.friction = 0.1f;
    b2BodyDef bd;
    bd.type = b2_dynamicBody;
    // subtracting -15 make the range effectively [-15, 15]
    bd.position.Set((int(dist30(rng)) - 15) + starter_center, 30);
    // set userData
    bd.userData.pointer = (uintptr_t)this;
    body = context->world->CreateBody(&bd);
    body->CreateFixture(&fd);

}

Block::~Block()
{
}
