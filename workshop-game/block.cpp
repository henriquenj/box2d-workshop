
#include <random>
#include <memory>

#include "block.h"
#include "text.h"
#include "draw_game.h"


Block::Block(GameContext* context, float starter_center)
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
    fd.density = 0.01f;
    fd.friction = 0.1f;
    b2BodyDef bd;
    bd.type = b2_dynamicBody;
    // subtracting -15 make the range effectively [-15, 15]
    bd.position.Set((int(dist30(rng)) - 15) + starter_center, 30);
    // set userData
    bd.userData.pointer = (uintptr_t)this;
    body = context->world->CreateBody(&bd);
    body->CreateFixture(&fd);
    body->SetAngularVelocity(2.0f);

    time_to_live = 300;
    // despawns after 1 second of being killed
    time_to_live_dying = 40;
    state = ALIVE;

}

Block::~Block()
{
}

void Block::Update()
{
    if (state == ALIVE)
    {
        // block live for a few seconds and then despawn
        time_to_live--;
        if (time_to_live == 0)
        {
            b2Vec2 pos = body->GetPosition();
            shouldDelete = true;
            // make player lose one live
            game_context->lives--;
            // spawn text where this Block died
            game_context->to_create.push_back(std::make_unique<FloatingText>(&g_debugDraw,
                                                                             pos.x,
                                                                             pos.y,
                                                                             "-1",
                                                                             ImColor(0.85, 0.00f, 0.00f),
                                                                             0.1f /* speed */,
                                                                             60 /* ttl */));
        }
    }
    else if (state == DYING)
    {
        time_to_live_dying--;
        if (time_to_live_dying <= 0)
        {
            // kills it
            shouldDelete = true;
        }
    }
}

void Block::OnCollision(PhysicalGameObject* other, b2Contact* contact)
{
    if (!contact->IsTouching())
    {
        // if it is not touching, don't process it.
        return;
    }
    // if collides with a bullet, kills the bullet and the block
    if (other->GetGameObjectType() == GameObjectType::BULLET)
    {
        // set DYING state
        state = DYING;
        // kill bullet
        other->Destroy();
    }
}
