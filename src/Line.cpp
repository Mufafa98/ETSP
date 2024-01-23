#include "Line.hpp"

Line::Line()
{
    thickness = 2;
    double xa = 10, xb = 100;
    double ya = 10, yb = 100;

    double length = sqrt((xb - xa) * (xb - xa) + (yb - ya) * (yb - ya));
    line.setSize(Vector2f(length, thickness));
}

void Line::Between(const Vector2f &first_pos, const Vector2f &second_pos)
{
    double xa = first_pos.x, xb = second_pos.x;
    double ya = first_pos.y, yb = second_pos.y;

    double length = sqrt((xb - xa) * (xb - xa) + (yb - ya) * (yb - ya));
    line.setSize(Vector2f(length, thickness));
    line.setPosition(first_pos);
    line.setRotation(atan2((second_pos.y - first_pos.y), (second_pos.x - first_pos.x)) * 180.0 / M_PI);
}

void Line::Draw(RenderWindow &window)
{
    window.draw(line);
}