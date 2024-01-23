#pragma once

#include <iostream>
#include <map>
#include <vector>

#include "CustomUtility.hpp"

using namespace std;

class Chromosome
{
private:
    map<pair<int, int>, int> *precomputed;
    size_t data_size;
    vector<int> data;

public:
    Chromosome(const size_t _data_size, map<pair<int, int>, int> *precomputed_distances, unsigned int &seed, const bool nn);

    unsigned int Evaluate();
};