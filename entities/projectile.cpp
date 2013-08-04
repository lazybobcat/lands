#include "projectile.h"
#include "../datatables.h"
#include "../utils.h"

namespace
{
    std::vector<ProjectileData> Table = initializeProjectileData();
}

Projectile::Projectile(Type type, TextureHolder &textures) :
    Entity(textures, Category::AlliedBullet),
    mType(type),
    mSprite(textures.get(Table[type].texture)),
    mDamages(Table[mType].damages)
{
    centerOrigin(mSprite);
}


void Projectile::drawCurrent(sf::RenderTarget &target, sf::RenderStates states) const
{
    target.draw(mSprite, states);
}


void Projectile::updateCurrent(sf::Time dt, CommandQueue &/*commands*/)
{
    Entity::Direction dir = getDirection();
    switch(dir)
    {
        case Up:
            SceneNode::move(sf::Vector2f(0.f, -Table[mType].speed*dt.asSeconds()));
            break;

        case Down:
            SceneNode::move(sf::Vector2f(0.f, Table[mType].speed*dt.asSeconds()));
            break;

        case Left:
            SceneNode::move(sf::Vector2f(-Table[mType].speed*dt.asSeconds(), 0.f));
            break;

        case Right:
            SceneNode::move(sf::Vector2f(Table[mType].speed*dt.asSeconds(), 0.f));
            break;
    }
}


sf::FloatRect Projectile::getBoundingRect() const
{
    return getWorldTransform().transformRect(mSprite.getGlobalBounds());
}

int Projectile::getDamages() const
{
    return mDamages;
}

unsigned int Projectile::getCategory() const
{
    return (mType == AlliedBullet) ? Category::AlliedBullet : Category::EnemyBullet;
}


bool Projectile::isCollidable() const
{
    return true;
}
