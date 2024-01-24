#pragma once

#include <iostream>
#include <map>
#include <cmath>
#include "ConstantData.hpp"
#include "CustomUtility.hpp"
#include "Chromosome.hpp"

using namespace std;

class SA
{
private:
    unsigned int seed;
    double temperature;
    Parameters parameters;
    map<pair<int, int>, int> *precomputed_distances;
    Chromosome *current_candidate;
    vector<int> past_solutions;
    EngineGUIData *data;

    int current_energy;
    int best_energy;

    void InitializeTemperature();
    void Evaluate();
    void DecrementTemperature();
    double StdDeviation(vector<int> past_results);

    void UpdatePermutationInData();

public:
    SA(const Parameters &_parameters, EngineGUIData *_data);

    int Run();
};