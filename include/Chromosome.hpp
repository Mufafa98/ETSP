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

    void _2OptMutation(unsigned int &seed);
    void OrderedCrossover(unsigned int &seed, Chromosome &first_child, Chromosome &second_child, Chromosome &second_parent);

public:
    Chromosome(const size_t _data_size, map<pair<int, int>, int> *precomputed_distances, unsigned int &seed, const bool nn);

    void PerformMutation(unsigned int &seed);
    void PerformCrossover(unsigned int &seed, Chromosome &first_child, Chromosome &second_child, Chromosome &second_parent);

    double SelectRandomCity(unsigned int &seed);
    double SelectDifRandomCity(unsigned int &seed, const double &different_than);
    double SelectNextCity(const double &city);
    double SelectPrevCity(const double &city);

    void ApplyInverOver(const double &city_c1, const double &city_c2);

    vector<int> GetData();
    size_t GetDataSize();

    unsigned int Evaluate();
};