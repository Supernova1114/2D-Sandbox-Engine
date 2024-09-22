#include <iostream>
#include <string>
#include <memory>
#include <chrono>
#include <thread>
#include "Particle.hpp"
#include "Map2D.hpp"

// Project to explore creating a 2D sandbox engine.
// Particles can react to gravity, etc, can be mixed to create new elements.

// TODO - Re-implemment immovable particles.




int main()
{
    std::cout << "\033[2J\033[1;1H"; // Clear entire screen.
    
    // Create a map for the sandbox.
    
    int MAP_WIDTH = 100;
    int MAP_HEIGHT = 50;
    std::string mapEmptyIcon = " ";

    // std::vector<Particle*> particleList(0); // Used for update loop.

    Map2D map(MAP_WIDTH, MAP_HEIGHT, mapEmptyIcon);


    int count = 0;
    //long timeout = 2;
    //long prevTime = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());

    for (int row = 10; row < MAP_HEIGHT; row++)
    {
        for (int col = 10; col < 20; col++)
        {
            map.addParticle(col, row, ".", true);
        }

        for (int col = 80; col < 90; col++)
        {
            map.addParticle(col, row, ".", true);
        }
    }

    while (count < 1500)
    {
        if (count % 2 == 0 && (count < 780))
        {
            map.addParticle(5, 0, "x", false);
            map.addParticle(95, 0, "o", false);
            
        }

        if (count == 850)
        {
            for (int row = 10; row < MAP_HEIGHT; row++)
            {
                for (int col = 10; col < 20; col++)
                {
                    map.removeParticle(col, row);
                }

                for (int col = 80; col < 90; col++)
                {
                    map.removeParticle(col, row);
                }
            }
        }
        

        // if (count % 500 == 0 && count > 1800 && count < 3600)
        // {
        //     for (int row = 20; row < MAP_HEIGHT; row++)
        //     {
        //         for (int col = MAP_WIDTH - 30; col < MAP_WIDTH; col++)
        //         {
        //             map.removeParticle(col, row);
        //         }
        //     }
        // }

        std::this_thread::sleep_for(std::chrono::milliseconds(7));

        printf("\033[%d;%dH", 1, 1); // Move cursor to top left.

        
        map.printMap();

        map.update();

        count++;
    }

    //map.printMap(); 

    // Spawn particles

    // Particle* particle = new Particle(1, 1, "x");

    // map.addParticle(particle);

    // delete(particle);
    // particle = nullptr;




    return 0;
}