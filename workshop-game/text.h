

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

    // X and Y startup position of the text and its contents.
    Text(DebugDraw* _draw,int _x, int _y, const std::string& _text, const ImColor& _color);

    // Render the text on the screen using DebugDraw
    void Update();
    virtual ~Text() = default;

private:
    std::string text;
    ImColor color;
    int x,y;
    DebugDraw* draw;
};
