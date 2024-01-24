#include "GA.hpp"

GA::GA(const Parameters &_parameters, EngineGUIData *_data)
{
    parameters = _parameters;
    seed = 314159265;
    best_energy = numeric_limits<int>::max();
    generations_with_no_improvement = 0;
    generation = 0;
    no_of_inversions = 0;

    decrement_time = parameters.max_no_of_generations / 6;

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

    for (size_t i = 0; i < parameters.pop_size; i += 2)
        cout << '_';
    cout << '\n';
    size_t _chromosome_size = instance.size();
    for (size_t i = 0; i < parameters.pop_size; i++)
    {
        if (i % 2 == 0)
        {
            cout << '#';
            cout.flush();
        }
        population.push_back(Chromosome(_chromosome_size, precomputed_distances, seed, 1));
    }
    cout << '\n';
}

vector<double> GA::UpdateProbability()
{
    int min_eval = numeric_limits<int>::max();
    vector<int> evaluation;
    const unsigned int pop_size = population.size();
    for (size_t i = 0; i < pop_size; i++)
    {
        evaluation.push_back(population[i].Evaluate());
        if (evaluation[i] < min_eval)
            min_eval = evaluation[i];
    }
    for (size_t i = 0; i < pop_size; i++)
    {
        evaluation[i] -= min_eval;
        evaluation[i] = 1 / (evaluation[i] + parameters.selection_preasure);
    }
    unsigned int total_energy = 0;
    for (size_t i = 0; i < pop_size; i++)
        total_energy += evaluation[i];
    vector<double> q;
    q.push_back(0);
    for (size_t i = 0; i < pop_size; i++)
        q.push_back(q[i] + evaluation[i] / total_energy);
    q.pop_back();
    q.push_back(1);
    return q;
}

void GA::PerformElitismSelection(vector<Chromosome> &new_population, const unsigned int &pop_size)
{
    vector<pair<Chromosome, int>> temp_pop;
    for (size_t i = 0; i < population.size(); i++)
        temp_pop.push_back(pair<Chromosome, int>(population[i], population[i].Evaluate()));
    sort(temp_pop.begin(), temp_pop.end(), [&](pair<Chromosome, int> &a, pair<Chromosome, int> &b)
         { return a.second < b.second; });
    for (size_t i = 0; i < pop_size; i++)
        new_population.push_back(temp_pop[i].first);
}

void GA::PerformRouletteSelection(vector<Chromosome> &new_population, const unsigned int &pop_size)
{
    vector<double> probability = UpdateProbability();
    const unsigned int original_pop_size = population.size();
    for (size_t i = 0; i < pop_size; i++)
    {
        double random_value = generateRandBetween(seed, 0, 1);
        for (size_t j = 0; j < original_pop_size; j++)
        {
            if (probability[j] < random_value && random_value <= probability[j + 1])
            {
                new_population.push_back(population[j]);
                break;
            }
        }
    }
}

void GA::PerformRandomSelection(vector<Chromosome> &new_population, const unsigned int &pop_size)
{
    const unsigned int original_pop_size = population.size();
    for (size_t i = 0; i < pop_size; i++)
    {
        const int random_value = generateIntRandBetween(seed, 0, original_pop_size - 1);
        new_population.push_back(population[random_value]);
    }
}

void GA::ApplySelection()
{
    vector<Chromosome> new_population;
    PerformElitismSelection(new_population, parameters.selection_by_elitism * parameters.pop_size);
    PerformRouletteSelection(new_population, parameters.pop_size - parameters.selection_by_elitism * parameters.pop_size);
    population.clear();
    population = new_population;
}

void GA::EvaluatePopulation()
{

    for (size_t i = 0; i < parameters.pop_size; i++)
    {
        int new_energy = population[i].Evaluate();
        if (best_energy > new_energy)
        {
            best_candidate = &population[i];
            generations_with_no_improvement = 0;
            best_energy = new_energy;
            UpdatePermutationInData();
        }
    }
}

void GA::ApplyMutation()
{
    if (parameters.mutation_rate == 0)
        return;
    for (size_t i = 0; i < population.size(); i++)
        if (generateRandBetween(seed, 0, 1) < parameters.mutation_rate)
            population[i].PerformMutation(seed);
}

void GA::ApplyCrossover()
{
    vector<Chromosome> parents;
    unsigned int max_parents = parameters.pop_size * parameters.crossover_rate;

    if (max_parents % 2 == 1)
    {
        if (__COUNTER__ % 2 == 0)
            max_parents++;
        else
            max_parents--;
    }
    PerformElitismSelection(parents, max_parents);

    while (!parents.empty())
    {
        Chromosome child1 = parents[0];
        Chromosome child2 = parents[1];
        parents[0].PerformCrossover(seed, child1, child2, parents[1]);
        parents.erase(parents.begin());
        parents.erase(parents.begin());
    }
}

void GA::UpdateParameters()
{
}

void GA::ShowOnScreen()
{
    cout << "[" << generation << "]\t" << best_energy << '\t' << '\t' << population.size() << '\t';
    cout << no_of_inversions << '\t' << generations_with_no_improvement << '\n';
}

void GA::UpdatePermutationInData()
{
    data->permutation.clear();
    data->permutaion_changed = true;
    vector<int> temp_candidate = best_candidate->GetData();
    data->permutation_size = temp_candidate.size();
    for (size_t i = 0; i < temp_candidate.size(); i++)
        data->permutation.push_back(temp_candidate[i] - 1);
}

void GA::ApplyInverOver()
{
    const double change_city = exp(log((0.5 - 0.2) / parameters.max_no_of_generations)) * generation;
    double city_c1, city_c2;
    for (Chromosome &individual : population)
    {
        Chromosome new_individual = individual;

        while (true)
        {
            if (generateRandBetween(seed, 0, 1) <= parameters.mutation_rate)
            {
                new_individual.PerformMutation(seed);
                city_c1 = new_individual.SelectRandomCity(seed);
            }

            if (generateRandBetween(seed, 0, 1) <= parameters.random_inversion)
                city_c2 = new_individual.SelectDifRandomCity(seed, city_c1);
            else
            {
                Chromosome temp_individual = population[generateIntRandBetween(seed, 0, population.size() - 1)];
                city_c2 = temp_individual.SelectNextCity(city_c1);
            }

            if (city_c2 == new_individual.SelectNextCity(city_c1) ||
                city_c2 == new_individual.SelectPrevCity(city_c1))
                break;
            no_of_inversions++;

            new_individual.ApplyInverOver(city_c1, city_c2);
            if (generateRandBetween(seed, 0, 1) <= change_city)
                city_c1 = city_c2;
        }
        if (new_individual.Evaluate() <= individual.Evaluate())
            individual = new_individual;
    }
}

double GA::Run()
{
    cout << fixed << setprecision(5);
    EvaluatePopulation();

    while (generation < parameters.max_no_of_generations)
    {
        if (data->window_closed)
            exit(1);
        ShowOnScreen();
        generation++;
        generations_with_no_improvement++;
        ApplyInverOver();

        ApplyCrossover();
        ApplySelection();
        EvaluatePopulation();

        UpdateParameters();
    }

    return best_energy;
}
