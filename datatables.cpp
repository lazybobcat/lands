#include "datatables.h"
#include "resources/particle.h"
#include "entities/entity.h"
#include "entities/projectile.h"
#include "entities/object.h"


std::vector<EntityData> initializeEntityData()
{
    std::vector<EntityData> data(Entity::TypeCount);

    data[Entity::Player].healthpoints = 100;
    data[Entity::Player].damages = 10;
    data[Entity::Player].speed = 200.f;
    data[Entity::Player].fireInterval = sf::seconds(0.5f);

    data[Entity::Enemy].healthpoints = 50;
    data[Entity::Enemy].damages = 50;
    data[Entity::Enemy].speed = 100.f;
    data[Entity::Enemy].fireInterval = sf::Time::Zero;

    return data;
}

std::vector<ProjectileData> initializeProjectileData()
{
    std::vector<ProjectileData> data(Projectile::TypeCount);

    data[Projectile::AlliedBullet].damages = 25;
    data[Projectile::AlliedBullet].speed = 300.f;
    data[Projectile::AlliedBullet].texture = Textures::LaserBullet;

    return data;
}

std::vector<ObjectData> initializeObjectData()
{
    std::vector<ObjectData> data(Object::TypeCount);

    data[Object::DoorBottom].texture = Textures::Objects;
    data[Object::DoorBottom].textureRect = sf::IntRect(64, 64, 64, 64);
    data[Object::DoorBottom].isCollidable = true;
    data[Object::DoorBottom].category = Category::Door;

    data[Object::DoorTop].texture = Textures::Objects;
    data[Object::DoorTop].textureRect = sf::IntRect(64, 0, 64, 64);
    data[Object::DoorTop].isCollidable = true;
    data[Object::DoorTop].category = Category::Door;

    data[Object::Table].texture = Textures::Objects;
    data[Object::Table].textureRect = sf::IntRect(0, 0, 64, 64);
    data[Object::Table].isCollidable = true;
    data[Object::Table].category = Category::Decoration;

    data[Object::Shit].texture = Textures::Objects;
    data[Object::Shit].textureRect = sf::IntRect(128, 0, 64, 64);
    data[Object::Shit].isCollidable = true;
    data[Object::Shit].category = Category::Decoration;

    data[Object::Key].texture = Textures::Objects;
    data[Object::Key].textureRect = sf::IntRect(192, 0, 64, 64);
    data[Object::Key].isCollidable = true;
    data[Object::Key].category = Category::Key;

    data[Object::TreeNW].texture = Textures::Objects;
    data[Object::TreeNW].textureRect = sf::IntRect(256, 0, 64, 64);
    data[Object::TreeNW].isCollidable = false;
    data[Object::TreeNW].category = Category::Decoration;

    data[Object::TreeNE].texture = Textures::Objects;
    data[Object::TreeNE].textureRect = sf::IntRect(320, 0, 64, 64);
    data[Object::TreeNE].isCollidable = false;
    data[Object::TreeNE].category = Category::Decoration;

    data[Object::Slime].texture = Textures::Objects;
    data[Object::Slime].textureRect = sf::IntRect(0, 64, 64, 64);
    data[Object::Slime].isCollidable = false;
    data[Object::Slime].category = Category::Decoration;

    data[Object::Mushroom].texture = Textures::Objects;
    data[Object::Mushroom].textureRect = sf::IntRect(128, 64, 64, 64);
    data[Object::Mushroom].isCollidable = true;
    data[Object::Mushroom].category = Category::Decoration;

    data[Object::Shrooms].texture = Textures::Objects;
    data[Object::Shrooms].textureRect = sf::IntRect(192, 64, 64, 64);
    data[Object::Shrooms].isCollidable = false;
    data[Object::Shrooms].category = Category::Decoration;

    data[Object::TreeSW].texture = Textures::Objects;
    data[Object::TreeSW].textureRect = sf::IntRect(256, 64, 64, 64);
    data[Object::TreeSW].isCollidable = true;
    data[Object::TreeSW].category = Category::Decoration;

    data[Object::TreeSE].texture = Textures::Objects;
    data[Object::TreeSE].textureRect = sf::IntRect(320, 64, 64, 64);
    data[Object::TreeSE].isCollidable = true;
    data[Object::TreeSE].category = Category::Decoration;

    return data;
}


std::vector<ParticleData> initializeParticleData()
{
    std::vector<ParticleData> data(Particle::ParticleCount);

    data[Particle::Default].color = sf::Color(255, 255, 50);
    data[Particle::Default].lifetime = sf::seconds(0.6f);

    return data;
}
