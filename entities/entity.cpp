#include "entity.h"
#include "scenenode.h"
#include "projectile.h"
#include "../events/commandqueue.h"
#include "../datatables.h"

#include <iostream>

namespace
{
    std::vector<EntityData> Table = initializeEntityData();
}

Entity::Entity(TextureHolder& textures, Category::Type cat) :
    SceneNode(cat),
    mHealthpoints(1),
    mDamages(0),
    mType(Enemy)
{
    switch(cat)
    {
        case Category::Player:
            mHealthpoints = Table[Entity::Player].healthpoints;
            mDamages = Table[Entity::Player].damages;
            mType = Player;
            break;

        case Category::Ghost:
            mHealthpoints = Table[Entity::Enemy].healthpoints;
            mDamages = Table[Entity::Enemy].damages;
            break;

        default:break;
    }

    // Commands
    mFireCommand.category = Category::EntityLayer;
    mFireCommand.action = [this, &textures] (SceneNode& node, sf::Time) {
        fire(node, textures);
    };
}

void Entity::updateCurrent(sf::Time dt, CommandQueue &commands)
{
    if(isDestroyed())
        return;

    if(mIsMoving)
    {
        mLastSafePosition = getWorldPosition();
        switch(mDirection)
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
        mIsMoving = false;
    }

    checkFiring(dt, commands);
}


void Entity::demove()
{
    setPosition(mLastSafePosition);
}

void Entity::stop()
{
    mIsMoving = false;
}


void Entity::fire()
{
    mIsFiring = true;
}

void Entity::fire(SceneNode& node, TextureHolder& textures)
{
    // Creation of projectile
    Projectile::Type type = (isAllied()) ? Projectile::AlliedBullet : Projectile::EnemyBullet;
    Projectile::Ptr projectile(new Projectile(type, textures));
    projectile->setDirection(getDirection());
    projectile->setPosition(getPosition());
    node.attachChild(std::move(projectile));
}

int Entity::getDamages() const
{
    return mDamages;
}

void Entity::checkFiring(sf::Time dt, CommandQueue &commands)
{
    if(mIsFiring && mFireCooldown <= sf::Time::Zero)
    {
        commands.push(mFireCommand);
        mFireCooldown += Table[mType].fireInterval;
        mIsFiring = false;
        playLocalSound(commands, Sounds::Shot);
    }
    else if(mFireCooldown > sf::Time::Zero)
    {
        mFireCooldown -= dt;
        mIsFiring = false;
    }
}




void Entity::move(Direction dir)
{
    setDirection(dir);
    mIsMoving = true;
}

void Entity::damage(int points)
{
    mHealthpoints -= points;
}

void Entity::heal(int points)
{
    mHealthpoints += points;
}

bool Entity::isMoving() const
{
    return mIsMoving;
}

void Entity::destroy()
{
    mHealthpoints = 0;
    mIsMoving = false;
    mIsFiring = false;
}

int Entity::getHealthpoints() const
{
    return mHealthpoints;
}

bool Entity::isDestroyed() const
{
    return (mHealthpoints <= 0);
}

void Entity::playLocalSound(CommandQueue &commands, Sounds::ID sound)
{
    Command command;
    command.category = Category::SoundEffect;
    command.action = derivedAction<SoundNode>(std::bind(&SoundNode::playSound, std::placeholders::_1, sound, getWorldPosition()));

    commands.push(command);
}

sf::FloatRect Entity::getBoundingRect() const
{
    return sf::FloatRect();
}

Entity::Direction Entity::getDirection() const
{
    return mDirection;
}

void Entity::setDirection(Direction dir)
{
    mDirection = dir;
}

bool Entity::isAllied() const { return false; }
bool Entity::isMarkedForRemoval() const { return isDestroyed(); }
