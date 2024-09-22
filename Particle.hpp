#pragma once

#include <string>

class Particle 
{
public:

Particle(int pos_x, int pos_y, std::string icon, bool isImmovable);

std::string getIcon();

int getX();
int getY();

void setPosition(int x, int y);

void update();

bool isImmovable();

bool moveParticle(int dest_x, int dest_y);

private:

    int pos_x;
    int pos_y;

    std::string icon;
    bool immovable;

};