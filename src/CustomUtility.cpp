#include "CustomUtility.hpp"

void DecodeEUC2D(const string path, map<int, Coords> &instance)
{
    ifstream file(path);

    if (!file.is_open())
    {
        cerr << "Error: Unable to open file at '" << path << "'" << endl;
        return;
    }

    string word;
    bool started_data_section = false;
    unsigned int counter = 0;
    double temp_memory[3] = {0, 0, 0};
    while (file >> word)
    {
        if (started_data_section && word != "EOF")
        {
            temp_memory[counter++] = stod(word);
            if (counter == 3)
            {
                counter = 0;
                instance.insert(pair<double, Coords>(temp_memory[0], Coords(temp_memory[1], temp_memory[2])));
            }
        }
        if (!started_data_section && word == "NODE_COORD_SECTION")
            started_data_section = true;
    }

    file.close();
}
void DecodeEUC2D(const string path, vector<Coords> &instance)
{
    ifstream file(path);

    if (!file.is_open())
    {
        cerr << "Error: Unable to open file at '" << path << "'" << endl;
        return;
    }

    string word;
    bool started_data_section = false;
    unsigned int counter = 0;
    double temp_memory[3] = {0, 0, 0};
    while (file >> word)
    {
        if (started_data_section && word != "EOF")
        {
            temp_memory[counter++] = stod(word);
            if (counter == 3)
            {
                counter = 0;
                instance.push_back(Coords(temp_memory[1], temp_memory[2]));
            }
        }
        if (!started_data_section && word == "NODE_COORD_SECTION")
            started_data_section = true;
    }

    file.close();
}
double generateRandBetween(unsigned int &seed, double min_value, double max_value)
{
    mt19937 generator(seed);
    uniform_real_distribution<double> distribution(min_value, max_value);
    seed++;
    return distribution(generator);
}
int generateIntRandBetween(unsigned int &seed, int min_value, int max_value)
{
    mt19937 generator(seed);
    uniform_int_distribution<int> distribution(min_value, max_value);
    seed++;
    return distribution(generator);
}