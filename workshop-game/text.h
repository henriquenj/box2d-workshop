

#pragma once

#include <string>

#include "draw_game.h"
#include "imgui/imgui.h"

#include "game_object.h"

// A Text object that stores size and position information. Needs to be rendered
// with the Render() function
class Text : public GameObject
{
public:

    // X and Y startup position (in box2d coordinates) of the text and its contents.
    Text(DebugDraw* _draw,float _x, float _y, const std::string& _text, const ImColor& _color);

    // Render the text on the screen using DebugDraw
    virtual void Update() override;
    virtual ~Text() = default;

protected:
    std::string text;
    ImColor color;
    // Current position of the text in b2World coordinates
    float x,y;
    DebugDraw* draw;
};


// Text class that flies away
class FloatingText : public Text {
public:
    FloatingText(DebugDraw* _draw, float _x, float _y, const std::string& _text, const ImColor& _color, float _speed, int _ttl);
    virtual void Update() override;
    virtual ~FloatingText() = default;
private:
    // after how many frames should this text disappear
    int ttl;
    float speed;
};
