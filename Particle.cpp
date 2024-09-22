#include "Particle.hpp"

Particle::Particle(int pos_x, int pos_y, std::string icon, bool isImmovable)
{
    this->pos_x = pos_x;
    this->pos_y = pos_y;
    this->icon = icon;
    this->immovable = isImmovable;
}

std::string Particle::getIcon()
{
    return icon;
}

int Particle::getX() {return pos_x;}

int Particle::getY() {return pos_y;}

void Particle::setPosition(int x, int y)
{
    pos_x = x;
    pos_y = y;
}

void Particle::update()
{

}

bool Particle::isImmovable() {return immovable;}
