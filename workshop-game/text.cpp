
#include <text.h>

Text::Text(int x_pos, int y_pos, const std::string& input)
{
    x = x_pos;
    y = y_pos;
    text = input;
}

void Text::SetText(const std::string& input)
{
    text = input;
}

void Text::Render(const DebugDraw* draw)
{
}
