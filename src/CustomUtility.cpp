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
