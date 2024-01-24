#include <iostream>
#include <pthread.h>
#include <unistd.h>
#include <SFML/Graphics.hpp>

#include "ConstantData.hpp"
#include "Path.hpp"

#include "SA.hpp"
#include "GA.hpp"

using namespace std;
using namespace sf;

void *graphicalThread(void *arg)
{

    struct EngineGUIData *data = static_cast<struct EngineGUIData *>(arg);

    Path path(data->instance_name.c_str());

    RenderWindow window(VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), WINDOW_TITLE);
    window.setVerticalSyncEnabled(true);

    while (window.isOpen())
    {
        Event event;
        while (window.pollEvent(event))
        {
            switch (event.type)
            {
            case Event::Closed:
            {
                window.close();
                data->window_closed = true;
                break;
            }
            default:
                break;
            }
        }

        if (data->permutaion_changed)
        {
            path.UpdatePermutation(data->permutation, data->permutation_size);
            data->permutaion_changed = false;
        }
        window.clear();
        path.Draw(window);
        window.display();

        // Sleep for a short time to avoid high CPU usage
        usleep(10000); // 10 milliseconds
    }

    return nullptr;
}

int main()
{

    pthread_t gui_thread_id;
    struct EngineGUIData *data = new struct EngineGUIData;
    data->window_closed = false;

    data->instance_name = "dsj1000";
    cout << "Enter instance name:\n";
    cin >> data->instance_name;
    data->instance_name_size = data->instance_name.size();

    if (pthread_create(&gui_thread_id, nullptr, graphicalThread, static_cast<void *>(data)) != 0)
    {
        cerr << "Error creating graphical thread" << endl;
        return 1;
    }
    cout << "Select solving method(SA/GA):\n";
    string type;
    cin >> type;

    Parameters parameters;
    if (type == "SA")
    {
        parameters.temp_min = 0.01;
        parameters.number_of_trials = 1000;
        parameters.max_no_of_iterations = 1000;
        parameters.max_memory_size = 10;
        parameters.delta_prec = 0.001;

        string params_info = "The parameters are as folows:\n1. Minimum temperature = " + to_string(parameters.temp_min);
        params_info += "\n2. Number of trials = " + to_string(parameters.number_of_trials);
        params_info += "\n3. Number of iterations = " + to_string(parameters.max_no_of_iterations);
        params_info += "\n4. Number of past solutions to be remembered = " + to_string(parameters.max_memory_size);
        params_info += "\n5. Delta precision = " + to_string(parameters.delta_prec);
        params_info += "\nIn order to change one of the above parameters enter it's id and the value wanted, ";
        params_info += "otherwise input 0 and run it with the default parameters\n";
        cout << params_info;
        unsigned int command;
        cin >> command;
        while (command != 0)
        {
            switch (command)
            {
            case 1:
                cin >> parameters.temp_min;
                break;
            case 2:
                cin >> parameters.number_of_trials;
                break;
            case 3:
                cin >> parameters.max_no_of_iterations;
                break;
            case 4:
                cin >> parameters.max_memory_size;
                break;
            case 5:
                cin >> parameters.delta_prec;
                break;

            default:
                break;
            }
            cout << "Do you want to change something else?\nIf yes type the parameter id that you want to modify else type 0\n";
            cin >> command;
        }
        params_info = "The parameters are as folows:\n1. Minimum temperature = " + to_string(parameters.temp_min);
        params_info += "\n2. Number of trials = " + to_string(parameters.number_of_trials);
        params_info += "\n3. Number of iterations = " + to_string(parameters.max_no_of_iterations);
        params_info += "\n4. Number of past solutions to be remembered = " + to_string(parameters.max_memory_size);
        params_info += "\n5. Delta precision = " + to_string(parameters.delta_prec) + "\n";
        cout << params_info;
        SA engine(parameters, data);
        engine.Run();
    }
    else if (type == "GA")
    {
        parameters.pop_size = 200;
        parameters.max_no_of_generations = 10000;
        parameters.random_inversion = 0.3;
        parameters.mutation_rate = 0.25;
        parameters.crossover_rate = 0.8;
        parameters.selection_by_elitism = 0.4;
        parameters.selection_preasure = 0.0001;

        string params_info = "The parameters are as folows:\n1. Population size = " + to_string(parameters.pop_size);
        params_info += "\n2. Number of generations = " + to_string(parameters.max_no_of_generations);
        params_info += "\n3. Number of random inversions = " + to_string(parameters.random_inversion);
        params_info += "\n4. Mutation Rate = " + to_string(parameters.mutation_rate);
        params_info += "\n5. Crossover Rate = " + to_string(parameters.crossover_rate);
        params_info += "\n6. Selection by Elitism = " + to_string(parameters.selection_by_elitism);
        params_info += "\n7. Selection preasure = " + to_string(parameters.selection_preasure);
        params_info += "\nIn order to change one of the above parameters enter it's id and the value wanted, ";
        params_info += "otherwise input 0 and run it with the default parameters\n";
        cout << params_info;
        unsigned int command;
        cin >> command;
        while (command != 0)
        {
            switch (command)
            {
            case 1:
                cin >> parameters.pop_size;
                break;
            case 2:
                cin >> parameters.max_no_of_generations;
                break;
            case 3:
                cin >> parameters.random_inversion;
                break;
            case 4:
                cin >> parameters.mutation_rate;
                break;
            case 5:
                cin >> parameters.crossover_rate;
                break;
            case 6:
                cin >> parameters.selection_by_elitism;
                break;
            case 7:
                cin >> parameters.selection_preasure;
                break;
            default:
                break;
            }
            cout << "Do you want to change something else?\nIf yes type the parameter id that you want to modify else type 0\n";
            cin >> command;
        }
        params_info = "The parameters are as folows:\n1. Population size = " + to_string(parameters.pop_size);
        params_info += "\n2. Number of generations = " + to_string(parameters.max_no_of_generations);
        params_info += "\n3. Number of random inversions = " + to_string(parameters.random_inversion);
        params_info += "\n4. Mutation Rate = " + to_string(parameters.mutation_rate);
        params_info += "\n5. Crossover Rate = " + to_string(parameters.crossover_rate);
        params_info += "\n6. Selection by Elitism = " + to_string(parameters.selection_by_elitism);
        params_info += "\n7. Selection preasure = " + to_string(parameters.selection_preasure);
        params_info += "\nIn order to change one of the above parameters enter it's id and the value wanted, ";
        params_info += "otherwise input 0 and run it with the default parameters\n";
        cout << params_info;
        GA engine(parameters, data);
        engine.Run();
    }
    if (pthread_join(gui_thread_id, nullptr) != 0)
    {
        std::cerr << "Error joining graphical thread" << std::endl;
        return 1;
    }
    return 0;
}
