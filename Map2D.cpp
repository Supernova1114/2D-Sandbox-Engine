#include "Map2D.hpp"

// Constructor
Map2D::Map2D(int map_width, int map_height, std::string emptyIcon)
{
    this->map_width = map_width;
    this->map_height = map_height;
    this->emptyIcon = emptyIcon;

    instantiateMap(map_width, map_height);
}

// Instantiate map of size (map_width, map_height) with nullptr in each position.
void Map2D::instantiateMap(int map_width, int map_height)
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
bool Map2D::addParticle(int x, int y, std::string icon, bool isImmovable)
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
bool Map2D::removeParticle(int x, int y)
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
bool Map2D::moveParticle(std::shared_ptr<Particle> particle, int dest_x, int dest_y)
{
    if (isWithinMapBoundsAndEmpty(dest_x, dest_y) == true)
    {
        int curr_x = particle->getX();
        int curr_y = particle->getY();

        // Ignore if destination is same as current position.
        if (curr_x == dest_x && curr_y == dest_y)
        {
            return true;
        }

        map[curr_y][curr_x] = nullptr;
        particle->setPosition(dest_x, dest_y);
        map[dest_y][dest_x] = particle;

        return true;
    }

    return false;
}

bool Map2D::swapParticle(std::shared_ptr<Particle> particle, int dest_x, int dest_y)
{
    if (isWithinMapBounds(dest_x, dest_y))
    {
        if (isPositionEmpty(dest_x, dest_y))
        {
            return moveParticle(particle, dest_x, dest_y);
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
bool Map2D::isPositionEmpty(int x, int y)
{
    return map[y][x] == nullptr;
}

// Print the current state of the map to the terminal.
void Map2D::printMap()
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
bool Map2D::isWithinMapBounds(int x, int y)
{
    return (x >= 0 && x < map_width && y >= 0 && y < map_height);
}

// Check if position is within map bounds and is empty.
bool Map2D::isWithinMapBoundsAndEmpty(int x, int y)
{
    return isWithinMapBounds(x, y) && isPositionEmpty(x, y);
}

// Get particle at (x,y) position.
std::shared_ptr<Particle> Map2D::getParticle(int x, int y)
{
    return isWithinMapBounds(x, y) ? map[y][x] : nullptr;
}

int Map2D::getMapWidth() {return map_width;}

int Map2D::getMapHeight() {return map_height;}

int Map2D::getPseudoRandomInt(int min, int max)
{
    return min + ( std::rand() % ( max - min + 1 ) );
}

void Map2D::update()
{
    for (std::shared_ptr<Particle> particle : particleList)
    {
        //particle->update();

        int current_x = particle->getX();
        int current_y = particle->getY();

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