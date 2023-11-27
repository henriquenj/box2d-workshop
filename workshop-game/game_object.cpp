


#include "game_object.h"


GameObject::GameObject()
{
}

GameObject::~GameObject()
{
}

void GameObject::Update()
{
    // Does nothing on base class
}

void GameObject::OnKeyPress(int, int, int, int)
{
    // Does nothing on base class
}

void GameObject::OnMousePress(int32, int32, int32)
{
    // Does nothing on base class
}

// PhysicalGameObject functions
PhysicalGameObject::PhysicalGameObject(GameContext* context)
    : body(nullptr), game_context(context)
{
}

// Move constructor
PhysicalGameObject::PhysicalGameObject(PhysicalGameObject&& other)
{
    // copy pointer to b2Body and nullify other's body (so it is not deleted
    // twice)
    body = other.body;
    // update userData, so Box2D has an updated reference for this object
    if (body != nullptr)
    {
        b2BodyUserData& userData = body->GetUserData();
        userData.pointer = (uintptr_t)this;
    }
    other.body == nullptr;
    game_context = other.game_context;
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
        game_context = other.game_context;
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


void PhysicalGameObject::OnCollision(PhysicalGameObject* other, b2Contact* contact)
{
    // Does nothing on base class
}
