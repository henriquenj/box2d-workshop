
#include <text.h>

Text::Text(DebugDraw* _draw, float _x, float _y, const std::string& _text, const ImColor& _color)
{
    x = _x;
    y = _y;
    text = _text;
    color = _color;
    draw = _draw;
}

void Text::Update()
{
    // convert from box2d coordinates to camera coordinates before drawing on
    // the screen
    b2Vec2 pos_window = g_camera.ConvertWorldToScreen(b2Vec2(x, y));
    draw->DrawString(pos_window.x, pos_window.y, color, text.c_str());
}


FloatingText::FloatingText(DebugDraw* _draw,float _x, float _y, const std::string& _text,
                           const ImColor& _color, float _speed, int _ttl)
    : Text(_draw, _x, _y, _text, _color)
{
    speed = _speed;
    ttl = _ttl;
}

void FloatingText::Update()
{
    // Call the base update but also increment Y position
    Text::Update();

    y += speed;

    ttl--;
    // make them disappear when time-to-live reaches 0
    if (ttl <= 0)
    {
        shouldDelete = true;
    }
}
