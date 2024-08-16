#include <iostream>
#include <vector>
#include <string>
#include <memory>
#include <chrono>
#include <thread>

// Project to explore creating a 2D sandbox engine.
// Particles can react to gravity, etc, can be mixed to create new elements.

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

    int pos_x; // Col
    int pos_y; // Row

    std::string icon;
    
};

class Map2D
{
public:

    Map2D(int map_width, int map_height)
    {
        this->map_width = map_width;
        this->map_height = map_height;

        map = std::vector<std::vector<std::shared_ptr<Particle>>>(map_height);

        // Instantiate map of size map_width, map_height with nullptr.
        for (int row = 0; row < map.size(); row++)
        {
            map[row] = std::vector<std::shared_ptr<Particle>>(map_width);
        
            for (int col = 0; col < map[row].size(); col++)
            {
                map[row][col] = nullptr;

            }
        }
    }

    // Add particle to empty position.
    bool addParticle(int x, int y)
    {
        if (map[y][x] == nullptr)
        {
            auto particle = std::shared_ptr<Particle>(new Particle(x, y, "o"));
            
            particleList.push_back(particle);
            map[y][x] = particle;

            return true;
        }

        return false;
    }

    bool moveParticle(std::shared_ptr<Particle> particle, int dest_x, int dest_y)
    {
        if (isWithinMapBoundsXY(dest_x, dest_y) && map[dest_y][dest_x] == nullptr)
        {
            map[particle->getY()][particle->getX()] = nullptr;
            particle->setPosition(dest_x, dest_y);
            map[dest_y][dest_x] = particle;

            return true;
        }

        return false;
    }

    void printMap()
    {
        for (int row = 0; row < map.size(); row++)
        {
            for (int col = 0; col < map[row].size(); col++)
            {
                if (map[row][col] != nullptr)
                {
                    std::cout << map[row][col]->getIcon();
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

    bool isWithinMapBoundsXY(int x, int y)
    {
        if (isWithinMapBoundsX(x) && isWithinMapBoundsY(y))
        {
            return true;
        }

        return false;
    }

    bool isWithinMapBoundsX(int x)
    {
        if (x >= 0 && x < map[0].size())
        {
            return true;
        }
        
        return false;
    }

    bool isWithinMapBoundsY(int y)
    {
        if (y >= 0 && y < map.size())
        {
            return true;
        }

        return false;
    }

    void testClear()
    {
        for (int row = map_height - 3; row < map_height; row++)
        {
            for (int col = 0; col < map_width; col++)
            {
                std::shared_ptr<Particle> particle = map[row][col];
                //particleList.erase(std::remove(particleList.begin(), particleList.end(), particle), particleList.end());
                map[row][col] = nullptr;
                particle = nullptr;
            }
        }
    }

    std::shared_ptr<Particle> getParticle(int x, int y)
    {
        if (isWithinMapBoundsXY(x, y))
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

            // See if below is clear
            if (getParticle(current_x, current_y + 1) == nullptr)
            {
                moveParticle(particle, current_x, current_y + 1);
            }
            else if (getParticle(current_x - 1, current_y) == nullptr && getParticle(current_x - 1, current_y + 1) == nullptr)
            {
                moveParticle(particle, current_x - 1, current_y);
            }
            else if (getParticle(current_x + 1, current_y) == nullptr && getParticle(current_x + 1, current_y + 1) == nullptr)
            {
                moveParticle(particle, current_x + 1, current_y);
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
        if (count % 2 == 0)
        {
            map.addParticle(5, 0);
            // map.addParticle(10, 0);
            // map.addParticle(15, 0);
            map.addParticle(20, 0);
            map.addParticle(25, 0);
            // map.addParticle(30, 0);
            // map.addParticle(35, 0);
            map.addParticle(40, 0);
            map.addParticle(45, 0);
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