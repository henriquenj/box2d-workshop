
#include <text.h>

Text::Text(DebugDraw* _draw, int _x, int _y, const std::string& _text, const ImColor& _color)
{
    x = _x;
    y = _y;
    text = _text;
    color = _color;
    draw = _draw;
}

void Text::Update()
{
    draw->DrawString(x, y, color, text.c_str());
}


FloatingText::FloatingText(DebugDraw* _draw,int _x, int _y, const std::string& _text, const ImColor& _color)
    : Text(_draw, _x, _y, _text, _color)
{
}

void FloatingText::Update()
{
    // Call the base update but also increment Y position
    Text::Update();

    y-=2;

    // make them disappear when they go off screen
    if (y < -10)
    {
        shouldDelete = true;
    }
}
