#ifndef DATATABLES_H
#define DATATABLES_H

#include <SFML/System/Time.hpp>
#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/Rect.hpp>

#include <vector>
#include <functional>

#include "resources/resourceidentifiers.h"
#include "events/category.h"


struct EntityData
{
    int         healthpoints;
    int         damages;
    float       speed;
    sf::Time    fireInterval;
};

std::vector<EntityData> initializeEntityData();



struct ProjectileData
{
    int             damages;
    float           speed;
    Textures::ID    texture;
};

std::vector<ProjectileData> initializeProjectileData();


struct ObjectData
{
    Textures::ID    texture;
    sf::IntRect     textureRect;
    bool            isCollidable;
    Category::Type  category;
};

std::vector<ObjectData> initializeObjectData();



struct ParticleData
{
    sf::Color   color;
    sf::Time    lifetime;
};

std::vector<ParticleData>	initializeParticleData();


#endif // DATATABLES_H
