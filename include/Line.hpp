#pragma once

#include <iostream>
#include <SFML/Graphics.hpp>
#include <cmath>

using namespace std;
using namespace sf;

class Line
{
private:
    unsigned int thickness;
    RectangleShape line;

public:
    Line();

    void Between(const Vector2f &first_pos, const Vector2f &second_pos);

    void Draw(RenderWindow &window);
};