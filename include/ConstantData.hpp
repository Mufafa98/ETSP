#pragma once

#include <vector>
#include <string>

using namespace std;

#define WINDOW_WIDTH 600
#define WINDOW_HEIGHT 600
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

struct Parameters
{
    // GA parameters
    double max_no_of_generations;
    double pop_size;
    double selection_preasure;
    double selection_by_elitism;
    double mutation_rate;
    double crossover_rate;
    double random_inversion;
    // SA parameters
    double temp_min;
    double max_no_of_iterations;
    double max_memory_size;
    double number_of_trials;
    double delta_prec;
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