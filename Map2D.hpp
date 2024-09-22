#pragma once

#include <string>
#include <memory>
#include <vector>
#include <iostream>
#include <cstdlib>
#include <random>
#include <algorithm>
#include "Particle.hpp"

class Map2D
{
public:

    // Constructor
    Map2D(int map_width, int map_height, std::string emptyIcon);

    // Instantiate map of size (map_width, map_height) with nullptr in each position.
    void instantiateMap(int map_width, int map_height);

    // Add particle to an empty position.
    // Returns true if successful.
    bool addParticle(int x, int y, std::string icon, bool isImmovable);

    // Remove particle from (x,y) position.
    bool removeParticle(int x, int y);

    // Move particle to empty location.
    // Returns true if successful.
    bool moveParticle(std::shared_ptr<Particle> particle, int dest_x, int dest_y);

    bool swapParticle(std::shared_ptr<Particle> particle, int dest_x, int dest_y);

    // Check if position within map is empty.
    bool isPositionEmpty(int x, int y);

    // Print the current state of the map to the terminal.
    void printMap();

    // Check if position is within map bounds.
    bool isWithinMapBounds(int x, int y);

    // Check if position is within map bounds and is empty.
    bool isWithinMapBoundsAndEmpty(int x, int y);

    // Get particle at (x,y) position.
    std::shared_ptr<Particle> getParticle(int x, int y);

    int getMapWidth();

    int getMapHeight();

    int getPseudoRandomInt(int min, int max);

    void update();

private:
    int map_width;
    int map_height;

    std::vector<std::vector<std::shared_ptr<Particle>>> map;
    std::vector<std::shared_ptr<Particle>> particleList;

    std::string emptyIcon;
};