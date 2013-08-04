#ifndef PROJECTILE_H
#define PROJECTILE_H

#include "entity.h"
#include "../resources/resourceidentifiers.h"

class Projectile : public Entity
{
public:
    enum Type
    {
        AlliedBullet,
        EnemyBullet,
        TypeCount
    };

    typedef std::unique_ptr<Projectile> Ptr;

public:
    Projectile(Type type, TextureHolder& textures);

    virtual unsigned int    getCategory() const;
    virtual sf::FloatRect   getBoundingRect() const;

    virtual bool            isCollidable() const;
    virtual int             getDamages() const;

private:
    virtual void drawCurrent(sf::RenderTarget &target, sf::RenderStates states) const;
    virtual void updateCurrent(sf::Time dt, CommandQueue &commands);

private:
    Type            mType;
    sf::Sprite      mSprite;
    sf::Vector2f    mDirection;
    int             mDamages;
};

#endif // PROJECTILE_H
