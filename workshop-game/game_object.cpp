


#include "game_object.h"


GameObject::GameObject(const GameContext* context)
{
    game_context = context;
}

GameObject::~GameObject()
{
}

void GameObject::Update()
{
}


// PhysicalGameObject functions
PhysicalGameObject::PhysicalGameObject(const GameContext* context)
    : GameObject(context), body(nullptr)
{
}

// Move constructor
PhysicalGameObject::PhysicalGameObject(PhysicalGameObject&& other)
    : GameObject(other.game_context)
{
    // copy pointer to b2Body and nullify other's body (so it is not deleted
    // twice)
    body = other.body;
    other.body == nullptr;
}

// Move assigment operator
PhysicalGameObject& PhysicalGameObject::operator=(PhysicalGameObject&& other)
{
    // prevent an object being moved onto itself
    if (this != &other)
    {
        // if there's any body associated with this object, destroy it now
        if (body != nullptr)
        {
            game_context->world->DestroyBody(body);
        }

        // copy the other body into this body and nullify the other's body
        body = other.body;
        other.body == nullptr;
    }

    return *this;
}

PhysicalGameObject::~PhysicalGameObject()
{
    if (body != nullptr)
    {
        // if there's a body defined for this game object, delete it now
        game_context->world->DestroyBody(body);
    }
}
