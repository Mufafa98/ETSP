#include "Chromosome.hpp"

Chromosome::Chromosome(const size_t _data_size, map<pair<int, int>, int> *precomputed_distances, unsigned int &seed, const bool nn)
{
    precomputed = precomputed_distances;
    data_size = _data_size;
    vector<int> temp;
    for (size_t i = 1; i <= _data_size; i++)
        temp.push_back(i);
    unsigned int random = generateIntRandBetween(seed, 0, temp.size() - 1);

    data.push_back(temp[random]);
    temp.erase(temp.begin() + random);

    while (data.size() < _data_size)
    {
        int best_energy = numeric_limits<int>::max();
        unsigned int best_candidate;
        for (size_t i = 0; i < temp.size(); i++)
        {
            if (temp[i] == temp[random])
                continue;
            int ii = temp[random];
            int jj = temp[i];
            if (ii > jj)
                swap(ii, jj);
            if (precomputed->at(pair<int, int>(ii, jj)) < best_energy)
            {
                best_energy = precomputed->at(pair<int, int>(ii, jj));
                best_candidate = i;
            }
        }
        data.push_back(temp[best_candidate]);
        temp.erase(temp.begin() + best_candidate);
    }
    data_size = data.size();
}

unsigned int Chromosome::Evaluate()
{
    int sum = 0;
    size_t i;
    for (i = 0; i < data.size() - 1; i++)
    {
        if (data[i + 1] > data[i])
            sum += precomputed->at(pair<int, int>(data[i], data[i + 1]));
        else
            sum += precomputed->at(pair<int, int>(data[i + 1], data[i]));
    }
    if (data[0] > data[i])
        sum += precomputed->at(pair<int, int>(data[i], data[0]));
    else
        sum += precomputed->at(pair<int, int>(data[0], data[i]));

    return sum;
}
