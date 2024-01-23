#include <iostream>
#include <pthread.h>
#include <unistd.h>
#include <SFML/Graphics.hpp>

#include "ConstantData.hpp"
#include "Path.hpp"

#include "SA.hpp"

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
    data->instance_name = "berlin52";
    data->instance_name_size = data->instance_name.size();

    if (pthread_create(&gui_thread_id, nullptr, graphicalThread, static_cast<void *>(data)) != 0)
    {
        cerr << "Error creating graphical thread" << endl;
        return 1;
    }

    Parameters parameters;
    parameters.temp_min = 0.01;
    parameters.number_of_trials = 1000;
    parameters.max_no_of_iterations = 1000;
    parameters.max_memory_size = 10;
    parameters.delta_prec = 0.001;

    SA engine(parameters, data->instance_name.c_str());

    if (pthread_join(gui_thread_id, nullptr) != 0)
    {
        std::cerr << "Error joining graphical thread" << std::endl;
        return 1;
    }
    return 0;
}
