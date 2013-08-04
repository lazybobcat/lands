#include "ghostentity.h"
#include "../utils.h"

GhostEntity::GhostEntity(GhostController &controller, TextureHolder &textures) :
    mController(controller),
    Entity(textures, Category::Ghost),
    mSprite(textures.get(Textures::Ghost), sf::IntRect(0, 0, 32, 32)),
    mDieAnimation(textures.get(Textures::GhostDead))
{
    mSprite.setOrigin(sf::Vector2f(16.f, 16.f));
    mDieAnimation.setFrameSize(sf::Vector2i(32,32));
    mDieAnimation.setNumFrames(2);
    mDieAnimation.setDuration(sf::seconds(1.f));
    mDieAnimation.setRepeating(true);
    mDieAnimation.setOrigin(sf::Vector2f(16.f, 16.f));
}

void GhostEntity::drawCurrent(sf::RenderTarget &target, sf::RenderStates states) const
{
    if(isDestroyed())
        target.draw(mDieAnimation, states);
    else
        target.draw(mSprite, states);
}

void GhostEntity::updateCurrent(sf::Time dt, CommandQueue &commands)
{
    mController.update(dt, commands, *this);

    if(isDestroyed())
        mDieAnimation.update(dt);
    else
        updateDirection();

    Entity::updateCurrent(dt, commands);
}

void GhostEntity::updateDirection()
{
    Entity::Direction dir = getDirection();

    switch(dir)
    {
        case Entity::Up:
        case Entity::Down:
            mSprite.setTextureRect(sf::IntRect(0,0,32,32));
            //mSprite.setOrigin(sf::Vector2f(16.f, 16.f));
            break;

        case Entity::Right:
            mSprite.setTextureRect(sf::IntRect(32,0,32,32));
            //mSprite.setOrigin(sf::Vector2f(16.f, 16.f));
            break;

        case Entity::Left:
            mSprite.setTextureRect(sf::IntRect(64,0,32,32));
            //mSprite.setOrigin(sf::Vector2f(16.f, 16.f));
            break;
    }
}

bool GhostEntity::isMarkedForRemoval() const
{
    //return Entity::isDestroyed() && mDieAnimation.isFinished();
    return false;
}

unsigned int GhostEntity::getCategory() const
{
    return Category::Ghost;
}


sf::FloatRect GhostEntity::getBoundingRect() const
{
    return getWorldTransform().transformRect(mSprite.getGlobalBounds());
}

bool GhostEntity::isCollidable() const
{
    return true;
}

