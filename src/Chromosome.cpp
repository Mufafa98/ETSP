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

void Chromosome::_2OptMutation(unsigned int &seed)
{
    unsigned int index1 = generateIntRandBetween(seed, 0, data.size() - 1);
    unsigned int index2;
    do
    {
        index2 = generateIntRandBetween(seed, 0, data.size() - 1);
    } while (index1 == index2);
    if (index1 > index2)
        swap(index1, index2);

    reverse(data.begin() + index1, data.begin() + index2 + 1);
}

void Chromosome::OrderedCrossover(unsigned int &seed, Chromosome &first_child, Chromosome &second_child, Chromosome &second_parent)
{
    data_size = data.size();
    unsigned int first_pos = generateIntRandBetween(seed, 0, data_size - 1);
    unsigned int second_pos = generateIntRandBetween(seed, 0, data_size - 1);
    while (first_pos == second_pos)
        second_pos = generateIntRandBetween(seed, 0, data.size());
    if (first_pos > second_pos)
        swap(first_pos, second_pos);
    first_child.data.clear();
    second_child.data.clear();

    for (size_t i = 0; i < data_size; i++)
    {
        if (!Exists(second_parent.data[i], data, first_pos, second_pos) && first_child.data.size() < first_pos)
            first_child.data.push_back(second_parent.data[i]);
        if (!Exists(data[i], second_parent.data, first_pos, second_pos) && second_child.data.size() < first_pos)
            second_child.data.push_back(data[i]);

        if (first_child.data.size() == data_size && second_child.data.size() == data_size)
            break;
    }
    for (size_t i = first_pos; i < second_pos; i++)
    {
        first_child.data.push_back(data[i]);
        second_child.data.push_back(second_parent.data[i]);
    }
    for (size_t i = 0; i < data_size; i++)
    {
        if (!Exists(second_parent.data[i], first_child.data, 0, first_child.data.size()) && first_child.data.size() < data_size)
            first_child.data.push_back(second_parent.data[i]);
        if (!Exists(data[i], second_child.data, 0, second_child.data.size()) && second_child.data.size() < data_size)
            second_child.data.push_back(data[i]);
    }
}

double Chromosome::SelectRandomCity(unsigned int &seed)
{
    return data[generateIntRandBetween(seed, 0, data.size() - 1)];
}

double Chromosome::SelectDifRandomCity(unsigned int &seed, const double &different_than)
{
    double new_city = different_than;
    while (new_city == different_than)
        new_city = data[generateIntRandBetween(seed, 0, data.size() - 1)];
    return new_city;
}

double Chromosome::SelectNextCity(const double &city)
{
    for (size_t i = 0; i < data.size() - 1; i++)
    {
        if (data[i] == city)
            return data[i + 1];
    }
    return data[0];
}

double Chromosome::SelectPrevCity(const double &city)
{
    for (size_t i = 1; i < data.size(); i++)
    {
        if (data[i] == city)
            return data[i - 1];
    }
    return data[data.size() - 1];
}

void Chromosome::ApplyInverOver(const double &city_c1, const double &city_c2)
{
    size_t possition_of_c1 = 0;
    size_t possition_of_c2 = 0;
    // find possition of c1
    data_size = data.size();
    for (size_t i = 0; i < data_size; i++)
    {
        if (data[i] == city_c1)
        {
            possition_of_c1 = i;
            break;
        }
    }
    // find possition of c2
    for (size_t i = 0; i < data_size; i++)
    {
        if (data[i] == city_c2)
        {
            possition_of_c2 = i;
            break;
        }
    }
    // update possition for inversion
    if (possition_of_c1 <= possition_of_c2)
    {
        possition_of_c1 = (possition_of_c1 + 1) % data_size;
        possition_of_c2 = (possition_of_c2 + 1) % data_size;
    }
    else if (possition_of_c1 > possition_of_c2)
    {
        swap(possition_of_c1, possition_of_c2);
        possition_of_c2--;
    }
    // apply inversion
    while (possition_of_c1 <= possition_of_c2)
    {
        swap(data[possition_of_c1], data[possition_of_c2]);
        possition_of_c1++;
        possition_of_c2--;
    }
}

void Chromosome::PerformMutation(unsigned int &seed)
{
    _2OptMutation(seed);
}

void Chromosome::PerformCrossover(unsigned int &seed, Chromosome &first_child, Chromosome &second_child, Chromosome &second_parent)
{
    OrderedCrossover(seed, first_child, second_child, second_parent);
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

vector<int> Chromosome::GetData()
{
    return data;
}

size_t Chromosome::GetDataSize()
{
    return data_size;
}
