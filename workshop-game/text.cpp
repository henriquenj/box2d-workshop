
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
