#pragma once

#include <vector>
#include <string>

using namespace std;

#define WINDOW_WIDTH 1200
#define WINDOW_HEIGHT 1000
#define WINDOW_TITLE "ETSP"

struct EngineGUIData
{
    bool window_closed;
    bool permutaion_changed;
    size_t permutation_size;
    size_t instance_name_size;
    vector<unsigned int> permutation;
    string instance_name;
};

struct Coords
{
    double x;
    double y;
    Coords(const double _x, const double _y)
    {
        x = _x;
        y = _y;
    }
};