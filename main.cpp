#include <iostream>
#include <vector>
#include <string>
#include <memory>
#include <chrono>
#include <thread>
#include <algorithm>
#include <cstdlib>

// Project to explore creating a 2D sandbox engine.
// Particles can react to gravity, etc, can be mixed to create new elements.

// TODO - move map and particle class to separate files from main.
// TODO - combine empty check and bounds check into a function potentially.
class Particle 
{
public:

    Particle(int pos_x, int pos_y, std::string icon)
    {
        this->pos_x = pos_x;
        this->pos_y = pos_y;
        this->icon = icon;
    }

    std::string getIcon()
    {
        return icon;
    }

    int getX() {return pos_x;}

    int getY() {return pos_y;}

    void setPosition(int x, int y)
    {
        pos_x = x;
        pos_y = y;
    }

    void update()
    {

    }

private:

    int pos_x;
    int pos_y;

    std::string icon;
    
};

class Map2D
{
public:

    // Constructor
    Map2D(int map_width, int map_height)
    {
        this->map_width = map_width;
        this->map_height = map_height;

        instantiateMap(map_width, map_height);
    }

    // Instantiate map of size (map_width, map_height) with nullptr in each position.
    void instantiateMap(int map_width, int map_height)
    {
        map = std::vector<std::vector<std::shared_ptr<Particle>>>(map_height);

        for (int row = 0; row < map_height; row++)
        {
            map[row] = std::vector<std::shared_ptr<Particle>>(map_width);
        
            for (int col = 0; col < map_width; col++)
            {
                map[row][col] = nullptr;
            }
        }
    }

    // Add particle to an empty position.
    // Returns true if successful.
    bool addParticle(int x, int y, std::string icon)
    {
        if (isWithinMapBounds(x, y) && isPositionEmpty(x, y))
        {
            auto particle = std::shared_ptr<Particle>(new Particle(x, y, icon));
            map[y][x] = particle;

            particleList.push_back(particle);

            return true;
        }

        return false;
    }

    // Remove particle from (x,y) position.
    bool removeParticle(int x, int y)
    {
        if (isWithinMapBounds(x, y))
        {
            if (!isPositionEmpty(x, y))
            {
                auto currentParticle = map[y][x];

                map[y][x] = nullptr;
                particleList.erase(std::remove(particleList.begin(), particleList.end(), currentParticle), particleList.end());
            }

            return true;
        }
        
        return false;
    }

    // Move particle to empty location.
    // Returns true if successful.
    bool moveParticle(std::shared_ptr<Particle> particle, int dest_x, int dest_y)
    {
        if (isWithinMapBounds(dest_x, dest_y) && isPositionEmpty(dest_x, dest_y))
        {
            map[particle->getY()][particle->getX()] = nullptr;
            particle->setPosition(dest_x, dest_y);
            map[dest_y][dest_x] = particle;

            return true;
        }

        return false;
    }

    // Check if position within map is empty.
    bool isPositionEmpty(int x, int y)
    {
        if (map[y][x] == nullptr)
        {
            return true;
        }

        return false;
    }

    // Print the current state of the map to the terminal.
    void printMap()
    {
        for (int row = 0; row < map_height; row++)
        {
            for (int col = 0; col < map_width; col++)
            {
                std::shared_ptr<Particle> currentParticle = map[row][col];

                if (currentParticle != nullptr)
                {
                    std::cout << currentParticle->getIcon();
                }
                else
                {
                    std::cout << ".";
                }

                std::cout << " ";
            }

            std::cout << std::endl;
        }
    }

    // Check if position is within map bounds.
    bool isWithinMapBounds(int x, int y)
    {
        if (x >= 0 && x < map_width
            && y >= 0 && y < map_height)
        {
            return true;
        }

        return false;
    }

    // Get particle at (x,y) position.
    std::shared_ptr<Particle> getParticle(int x, int y)
    {
        if (isWithinMapBounds(x, y))
        {
            return map[y][x];
        }

        return nullptr;
    }

    int getMapWidth() {return map_width;}

    int getMapHeight() {return map_height;}

    void update()
    {
        for (std::shared_ptr<Particle> particle : particleList)
        {
            

            //particle->update();

            int current_x = particle->getX();
            int current_y = particle->getY();

            // TODO - fix up stuffs

            if (isWithinMapBounds(current_x, current_y + 1) && isPositionEmpty(current_x, current_y + 1))
            {
                moveParticle(particle, current_x, current_y + 1);
            }
            else
            {
                // Choose between left or right at random.
                int randomDir = rand() % 2; // 0 or 1

                if (randomDir == 0)
                {
                    if (isWithinMapBounds(current_x - 1, current_y) && isWithinMapBounds(current_x - 1, current_y + 1) && isPositionEmpty(current_x - 1, current_y) && isPositionEmpty(current_x - 1, current_y + 1))
                    {
                        moveParticle(particle, current_x - 1, current_y);
                    }
                }
                else
                {
                    if (isWithinMapBounds(current_x + 1, current_y) && isWithinMapBounds(current_x + 1, current_y + 1) && isPositionEmpty(current_x + 1, current_y) && isPositionEmpty(current_x + 1, current_y + 1))
                    {
                        moveParticle(particle, current_x + 1, current_y);
                    }
                }
            }
        }
    }

private:
    int map_width;
    int map_height;

    std::vector<std::vector<std::shared_ptr<Particle>>> map;
    std::vector<std::shared_ptr<Particle>> particleList;
};


int main()
{
    // Create a map for the sandbox.
    
    int MAP_WIDTH = 50;
    int MAP_HEIGHT = 50;

    // std::vector<Particle*> particleList(0); // Used for update loop.

    Map2D map(MAP_WIDTH, MAP_HEIGHT);


    int count = 0;
    //long timeout = 2;
    //long prevTime = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());

    while (true)
    {
        if (count % 5 == 0 && count < 2000)
        {
            map.addParticle(5, 0, "#");
            // map.addParticle(10, 0);
            // map.addParticle(15, 0);
            map.addParticle(20, 0, "@");
            map.addParticle(25, 0, "o");
            // map.addParticle(30, 0);
            // map.addParticle(35, 0);
            map.addParticle(40, 0, "+");
            map.addParticle(45, 0, "%");
        }

        if (count % 10 == 0 && count > 1000)
        {
                map.removeParticle(25, 49);
                map.removeParticle(24, 49);
        }
        

        // if (std::chrono::system_clock::to_time_t(std::chrono::system_clock::now()) - prevTime > timeout)
        // {
        //     map.testClear();

        //     prevTime = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
        // }



        std::this_thread::sleep_for(std::chrono::milliseconds(10));
        std::cout << "\033[2J\033[1;1H";
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