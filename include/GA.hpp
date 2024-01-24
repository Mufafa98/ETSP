#pragma once
#include <iostream>
#include <iomanip>
#include <limits>
#include <chrono>
#include <algorithm>
#include "Chromosome.hpp"

using namespace std;

class GA
{
    unsigned int seed;
    unsigned int generation;
    unsigned int generations_with_no_improvement;
    unsigned int decrement_time;
    int best_energy;
    Chromosome *best_candidate;
    Parameters parameters;
    vector<Chromosome> population;
    map<pair<int, int>, int> *precomputed_distances;

    unsigned int no_of_inversions;

    EngineGUIData *data;

    void PerformRouletteSelection(vector<Chromosome> &new_population, const unsigned int &pop_size);
    void PerformRandomSelection(vector<Chromosome> &new_population, const unsigned int &pop_size);
    void PerformElitismSelection(vector<Chromosome> &new_population, const unsigned int &pop_size);
    vector<double> UpdateProbability();

    void EvaluatePopulation();
    void ApplyMutation();
    void ApplySelection();
    void ApplyCrossover();

    void ApplyInverOver();

    void UpdateParameters();

    void ShowOnScreen();

    void UpdatePermutationInData();

public:
    GA(const Parameters &_parameters, EngineGUIData *_data);

    double Run();
};