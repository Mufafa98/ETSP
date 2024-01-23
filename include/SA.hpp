#pragma once

#include <iostream>
#include <map>
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

    unsigned int current_energy;
    unsigned int best_energy;

    void InitializeTemperature();

public:
    SA(const Parameters &_parameters, const char *instance_name);

    void Run();
};