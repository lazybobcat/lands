#ifndef ENTITY_H
#define ENTITY_H

#include <SFML/System/Time.hpp>

#include "scenenode.h"
#include "soundnode.h"

class CommandQueue;

class Entity : public SceneNode
{
public:
    enum Direction
    {
        Down,
        Right,
        Left,
        Up
    };

    enum Type
    {
        Player,
        Allied,
        Enemy,
        TypeCount
    };

public:
    Entity(TextureHolder &textures, Category::Type cat = Category::None);

    void        damage(int points);
    void        heal(int points);
    void        destroy();
    int         getHealthpoints() const;
    virtual bool  isDestroyed() const;

    virtual sf::FloatRect getBoundingRect() const;

    void        playLocalSound(CommandQueue& commands, Sounds::ID sound);

    void        fire();
    void        fire(SceneNode& node, TextureHolder& textures);
    virtual int getDamages() const;

    Direction   getDirection() const;
    virtual void setDirection(Direction dir);

    virtual void move(Direction dir);
    void         demove(); // Soooooo ugly ....
    void         stop();

    virtual bool isAllied() const;
    virtual bool isMarkedForRemoval() const;
    bool         isMoving() const;

protected:
    virtual void    updateCurrent(sf::Time dt, CommandQueue& commands);

    void            checkFiring(sf::Time dt, CommandQueue& commands);


private:
    int         mHealthpoints;
    int         mDamages;
    Direction   mDirection = Down;
    Type        mType;
    bool        mIsMoving = false;
    bool        mIsFiring = false;
    sf::Time    mFireCooldown = sf::Time::Zero;

    Command     mFireCommand;
    sf::Vector2f mLastSafePosition;
};

#endif // ENTITY_H
