#include <SA.hpp>

SA::SA(const Parameters &_parameters, const char *instance_name)
{
    seed = 314159265;
    parameters = _parameters;

    string _path_to_test_instance = "./assets/" + string(instance_name) + ".tsp";

    map<int, Coords> instance;
    DecodeEUC2D(_path_to_test_instance, instance);

    precomputed_distances = new map<pair<int, int>, int>();
    for (size_t i = 1; i <= instance.size(); i++)
    {
        for (size_t j = i + 1; j <= instance.size(); j++)
        {
            double dx = instance.at(i).x - instance.at(j).x;
            double dy = instance.at(i).y - instance.at(j).y;
            int result = (int)(sqrt(dx * dx + dy * dy) + 0.5);
            precomputed_distances->insert(pair<pair<int, int>, int>(pair<int, int>(i, j), result));
        }
    }

    InitializeTemperature();

    size_t _chromosome_size = instance.size();

    current_candidate = new Chromosome(_chromosome_size, precomputed_distances, seed, 1);

    current_energy = current_candidate->Evaluate();
    best_energy = current_energy;
}

void SA::InitializeTemperature()
{
    temperature = 200;
}
