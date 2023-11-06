

#pragma once

#include <string>

#include "draw_game.h"

// A Text object that stores size and position information. Needs to be rendered
// with the Render() function
class Text
{
public:

    // X and Y startup position of the text and its contents.
    Text(int x_pos, int y_pos, const std::string& input);
    // Overwrite text that was provided initially
    void SetText(const std::string& input);

    // Render the text on the screen using DebugDraw
    void Render(const DebugDraw* draw);
    virtual ~Text() = default;

private:
    std::string text;
    int x,y;
};
