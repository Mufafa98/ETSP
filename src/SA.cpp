#include <SA.hpp>

SA::SA(const Parameters &_parameters, EngineGUIData *_data)
{
    seed = 314159265;
    parameters = _parameters;
    data = _data;

    string _path_to_test_instance = "./assets/" + string(data->instance_name) + ".tsp";

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
void SA::Evaluate()
{
    int result = current_candidate->Evaluate();
    if (result < best_energy)
    {
        best_energy = result;
        UpdatePermutationInData();
    }
}

void SA::UpdatePermutationInData()
{
    data->permutation.clear();
    data->permutaion_changed = true;
    vector<int> temp_candidate = current_candidate->GetData();
    data->permutation_size = temp_candidate.size();
    for (size_t i = 0; i < temp_candidate.size(); i++)
        data->permutation.push_back(temp_candidate[i] - 1);
}

double SA::StdDeviation(vector<int> past_results)
{
    double mean = 0;
    for (int element : past_results)
        mean += element;
    mean /= past_results.size();
    double std_deviation = 0;
    for (int element : past_results)
        std_deviation += (element - mean) * (element - mean);
    return sqrt(std_deviation / past_results.size());
}

void SA::DecrementTemperature()
{
    double std_deviation = StdDeviation(past_solutions);
    temperature = temperature / (1 + (log(1 + parameters.delta_prec) * temperature) / (3 * std_deviation));
}

void SA::InitializeTemperature()
{
    temperature = 200;
}

int SA::Run()
{
    Evaluate();
    do
    {
        if (data->window_closed)
            exit(1);
        for (int i = 0; i < parameters.max_no_of_iterations; i++)
        {
            Chromosome new_candidate = Chromosome(*current_candidate);
            new_candidate.PerformMutation(seed);
            int new_energy = new_candidate.Evaluate();
            if ((current_energy > new_energy) || generateRandBetween(seed, 0, 1) < exp(-abs(new_energy - current_energy) / temperature))
            {
                *current_candidate = Chromosome(new_candidate);
                current_energy = new_energy;
                past_solutions.push_back(current_energy);
                if (past_solutions.size() > parameters.max_memory_size)
                    past_solutions.erase(past_solutions.begin());
                if (best_energy > current_energy)
                {
                    best_energy = current_energy;
                    UpdatePermutationInData();
                }
            }
        }
        DecrementTemperature();
        cout << best_energy << ' ' << temperature << '\n';
    } while (temperature > parameters.temp_min);
    return best_energy;
}