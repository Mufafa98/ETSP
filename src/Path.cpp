#include "Path.hpp"

Path::Path(const char *instance)
{
    circle_radius = 5;
    string path = "./assets/";
    path += instance + string(".tsp");

    vector<Coords> temp_pos;
    DecodeEUC2D(path, temp_pos);
    double max_x = numeric_limits<double>::min();
    double max_y = numeric_limits<double>::min();
    for (size_t i = 0; i < temp_pos.size(); i++)
    {
        if (temp_pos[i].x > max_x)
            max_x = temp_pos[i].x;
        if (temp_pos[i].y > max_y)
            max_y = temp_pos[i].y;
    }
    for (size_t i = 0; i < temp_pos.size(); i++)
    {
        CircleShape circle;
        circle.setRadius(circle_radius);
        circle.setFillColor(Color::White);
        circle.setPosition(Vector2f(temp_pos[i].x / max_x * (WINDOW_WIDTH - 2 * circle_radius),
                                    temp_pos[i].y / max_y * (WINDOW_HEIGHT - 2 * circle_radius)));
        circles.push_back(circle);
    }
    for (size_t i = 1; i < temp_pos.size(); i++)
    {
        Line line;
        line.Between(circles[i - 1].getPosition() + Vector2f(5, 5),
                     circles[i].getPosition() + Vector2f(5, 5));
        lines.push_back(line);
    }
    Line line;
    line.Between(circles[0].getPosition() + Vector2f(5, 5),
                 circles[temp_pos.size() - 1].getPosition() + Vector2f(5, 5));
    lines.push_back(line);
}

void Path::UpdatePermutation(const vector<unsigned int> &permutation, const unsigned int &permutation_size)
{
    Line line;
    for (size_t i = 1; i < permutation_size; i++)
    {
        line.Between(circles[permutation[i - 1]].getPosition() + Vector2f(5, 5),
                     circles[permutation[i]].getPosition() + Vector2f(5, 5));
        lines.push_back(line);
    }
}

void Path::Draw(RenderWindow &window)
{
    for (size_t i = 0; i < circles.size(); i++)
    {
        window.draw(circles[i]);
    }
    for (size_t i = 0; i < lines.size(); i++)
    {
        lines[i].Draw(window);
    }
}
