#include <iostream>
#include <vector>
#include <string>
#include <memory>
#include <chrono>
#include <thread>
#include <algorithm>
#include <cstdlib>
#include <random>


// Project to explore creating a 2D sandbox engine.
// Particles can react to gravity, etc, can be mixed to create new elements.

// TODO - move map and particle class to separate files from main.
// TODO - combine empty check and bounds check into a function potentially.
class Particle 
{
public:

    Particle(int pos_x, int pos_y, std::string icon, bool isImmovable)
    {
        this->pos_x = pos_x;
        this->pos_y = pos_y;
        this->icon = icon;
        this->immovable = isImmovable;
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

    bool isImmovable() {return immovable;}

    void setPressure(int pressureX, int pressureY)
    {
        if (pressureX > 9)
        {
            pressureX = 9;
        }
        else if (pressureX < -9)
        {
            pressureX = -9;
        }

        if (pressureY > 9)
        {
            pressureY = 9;
        }
        else if (pressureY < -9)
        {
            pressureY = -9;
        }

        this->pressureX = pressureX;
        this->pressureY = pressureY;
        
        icon = std::to_string(abs(pressureX));
    }

    int getPressureX() {return pressureX;}
    int getPressureY() {return pressureY;}

private:

    int pos_x;
    int pos_y;

    std::string icon;
    bool immovable;

    int pressureX = 0;
    int pressureY = 0;
    
};

class Map2D
{
public:

    // Constructor
    Map2D(int map_width, int map_height, std::string emptyIcon)
    {
        this->map_width = map_width;
        this->map_height = map_height;
        this->emptyIcon = emptyIcon;

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
    bool addParticle(int x, int y, std::string icon, bool isImmovable)
    {
        if (isWithinMapBoundsAndEmpty(x, y))
        {
            auto particle = std::shared_ptr<Particle>(new Particle(x, y, icon, isImmovable));
            map[y][x] = particle;

            particleList.push_back(particle);

            return true;
        }

        return false;
    }

    // Remove particle from (x,y) position.
    bool removeParticle(int x, int y)
    {
        if (isWithinMapBounds(x, y) == true)
        {
            if (isPositionEmpty(x, y) == false)
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
        if (isWithinMapBoundsAndEmpty(dest_x, dest_y) == true
            && particle->isImmovable() == false)
        {
            if (particle->getX() == dest_x && particle->getY() == dest_y)
            {
                return true;
            }

            map[particle->getY()][particle->getX()] = nullptr;
            particle->setPosition(dest_x, dest_y);
            map[dest_y][dest_x] = particle;

            return true;
        }

        return false;
    }

    bool swapParticle(std::shared_ptr<Particle> particle, int dest_x, int dest_y)
    {
        if (particle->isImmovable() == true)
        {
            return false;
        }

        if (isWithinMapBounds(dest_x, dest_y))
        {
            if (isPositionEmpty(dest_x, dest_y))
            {
                moveParticle(particle, dest_x, dest_y);
            }
            else
            {
                auto tempParticle = map[dest_y][dest_x];

                if (tempParticle->isImmovable() == true)
                {
                    return false;
                }

                map[particle->getY()][particle->getX()] = tempParticle;
                tempParticle->setPosition(particle->getX(), particle->getY());

                particle->setPosition(dest_x, dest_y);
                map[dest_y][dest_x] = particle;
            }
            
            return true;
        }

        return false;
    }

    // Check if position within map is empty.
    bool isPositionEmpty(int x, int y)
    {
        return map[y][x] == nullptr;
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
                    std::cout << emptyIcon;
                }

                std::cout << " ";
            }

            std::cout << std::endl;
        }
    }

    // Check if position is within map bounds.
    bool isWithinMapBounds(int x, int y)
    {
        return (x >= 0 && x < map_width && y >= 0 && y < map_height);
    }

    // Check if position is within map bounds and is empty.
    bool isWithinMapBoundsAndEmpty(int x, int y)
    {
        return isWithinMapBounds(x, y) && isPositionEmpty(x, y);
    }

    // Get particle at (x,y) position.
    std::shared_ptr<Particle> getParticle(int x, int y)
    {
        return isWithinMapBounds(x, y) ? map[y][x] : nullptr;
    }

    int getMapWidth() {return map_width;}

    int getMapHeight() {return map_height;}

    int getPseudoRandomInt(int min, int max)
    {
        return min + ( std::rand() % ( max - min + 1 ) );
    }

    void update()
    {
        for (std::shared_ptr<Particle> particle : particleList)
        {
            //particle->update();

            int current_x = particle->getX();
            int current_y = particle->getY();

            int pressureX = particle->getPressureX();
            int pressureY = particle->getPressureY();

            if (moveParticle(particle, current_x, current_y + 1) == false)
            {
                int randomDirX = rand() % 9 - 4;
                int randomDirY = rand() % 5 - 2;
                if (swapParticle(particle, current_x + randomDirX, current_y) == false)
                {
                    if (getParticle(current_x, current_y + randomDirY) != nullptr)
                    {
                        swapParticle(particle, current_x, current_y + randomDirY);
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

    std::string emptyIcon;
};


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