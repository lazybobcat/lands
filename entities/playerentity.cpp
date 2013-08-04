#include "playerentity.h"
#include "../utils.h"

#include <iostream>

PlayerEntity::PlayerEntity(TextureHolder &textures) :
    Entity(textures, Category::Player),
    mSprite(textures.get(Textures::Player), sf::IntRect(0,0,32,32)),
    mDieAnimation(textures.get(Textures::PlayerDead)),
    mHealthbar(nullptr),
    mUIKey(nullptr)
{
    textures.get(Textures::Player).setSmooth(false);
    textures.get(Textures::PlayerWalkingDown).setSmooth(false);
    textures.get(Textures::PlayerWalkingUp).setSmooth(false);
    textures.get(Textures::PlayerWalkingLeft).setSmooth(false);
    textures.get(Textures::PlayerWalkingRight).setSmooth(false);
    textures.get(Textures::PlayerDead).setSmooth(false);

    mSprite.setOrigin(sf::Vector2f(16.f, 16.f));
    mSprite.setScale(sf::Vector2f(2.f, 2.f));

    mAnimations[Entity::Down].setTexture(textures.get(Textures::PlayerWalkingDown));
    mAnimations[Entity::Down].setFrameSize(sf::Vector2i(32,32));
    mAnimations[Entity::Down].setNumFrames(4);
    mAnimations[Entity::Down].setDuration(sf::seconds(0.7f));
    mAnimations[Entity::Down].setRepeating(true);
    mAnimations[Entity::Down].setOrigin(sf::Vector2f(16.f, 16.f));
    mAnimations[Entity::Down].setScale(sf::Vector2f(2.f, 2.f));

    mAnimations[Entity::Left].setTexture(textures.get(Textures::PlayerWalkingLeft));
    mAnimations[Entity::Left].setFrameSize(sf::Vector2i(32,32));
    mAnimations[Entity::Left].setNumFrames(4);
    mAnimations[Entity::Left].setDuration(sf::seconds(0.7f));
    mAnimations[Entity::Left].setRepeating(true);
    mAnimations[Entity::Left].setOrigin(sf::Vector2f(16.f, 16.f));
    mAnimations[Entity::Left].setScale(sf::Vector2f(2.f, 2.f));

    mAnimations[Entity::Right].setTexture(textures.get(Textures::PlayerWalkingRight));
    mAnimations[Entity::Right].setFrameSize(sf::Vector2i(32,32));
    mAnimations[Entity::Right].setNumFrames(4);
    mAnimations[Entity::Right].setDuration(sf::seconds(0.7f));
    mAnimations[Entity::Right].setRepeating(true);
    mAnimations[Entity::Right].setOrigin(sf::Vector2f(16.f, 16.f));
    mAnimations[Entity::Right].setScale(sf::Vector2f(2.f, 2.f));

    mAnimations[Entity::Up].setTexture(textures.get(Textures::PlayerWalkingUp));
    mAnimations[Entity::Up].setFrameSize(sf::Vector2i(32,32));
    mAnimations[Entity::Up].setNumFrames(4);
    mAnimations[Entity::Up].setDuration(sf::seconds(0.7f));
    mAnimations[Entity::Up].setRepeating(true);
    mAnimations[Entity::Up].setOrigin(sf::Vector2f(16.f, 16.f));
    mAnimations[Entity::Up].setScale(sf::Vector2f(2.f, 2.f));

    mDieAnimation.setFrameSize(sf::Vector2i(32,32));
    mDieAnimation.setNumFrames(1);
    mDieAnimation.setDuration(sf::seconds(10.f));
    mDieAnimation.setRepeating(true);
    mDieAnimation.setOrigin(sf::Vector2f(16.f, 16.f));
    mDieAnimation.setScale(sf::Vector2f(2.f, 2.f));

    // Children

    std::unique_ptr<Healthbar> hb(new Healthbar());
    hb->setPosition(7.f, -50.f);
    mHealthbar = hb.get();
    attachChild(std::move(hb));

    mAddUiCommand.category = Category::UILayer;
    mAddUiCommand.action = [this, &textures] (SceneNode& layer, sf::Time) {
        std::unique_ptr<UISprite> uikey(new UISprite(textures, Textures::UI, sf::IntRect(0,0,32,32)));
        uikey->setPosition(80.f, 80.f);
        uikey->hide();
        mUIKey = uikey.get();
        layer.attachChild(std::move(uikey));
    };
}


void PlayerEntity::updateCurrent(sf::Time dt, CommandQueue &commands)
{
    updateDirection();
    updateAnimation(dt);

    mHealthbar->setPercentage(getHealthpoints() / 100.f);

    if(!mUIKey)
    {
        commands.push(mAddUiCommand);
    }

    Entity::updateCurrent(dt, commands);
}


void PlayerEntity::drawCurrent(sf::RenderTarget &target, sf::RenderStates states) const
{
    if(isDestroyed())
    {
        if(mHealthbar)
            mHealthbar->setPercentage(0.f);
        target.draw(mDieAnimation, states);
        return;
    }

    if(mMovingAnimation)
    {
        target.draw(mAnimations[getDirection()], states);
    }
    else
    {
        target.draw(mSprite, states);
    }
}

void PlayerEntity::updateAnimation(sf::Time dt)
{
    if(isDestroyed())
    {
        mDieAnimation.update(dt);
    }
    else
    {
        mAnimations[getDirection()].update(dt);
    }

    mMovingAnimation = isMoving();
}

void PlayerEntity::updateDirection()
{
    switch(getDirection())
    {
        case Entity::Down:
            mSprite.setTextureRect(sf::IntRect(0,0,32,32));
            break;

        case Entity::Right:
            mSprite.setTextureRect(sf::IntRect(32,0,32,32));
            break;

        case Entity::Left:
            mSprite.setTextureRect(sf::IntRect(64,0,32,32));
            break;

        case Entity::Up:
            mSprite.setTextureRect(sf::IntRect(96,0,32,32));
            break;
    }
}


unsigned int PlayerEntity::getCategory() const
{
    return Category::Player;
}

sf::FloatRect PlayerEntity::getBoundingRect() const
{
    return getWorldTransform().transformRect(mSprite.getGlobalBounds());
}

bool PlayerEntity::isCollidable() const
{
    return true;
}

bool PlayerEntity::isAllied() const
{
    return true;
}

bool PlayerEntity::isMarkedForRemoval() const
{
    return isDestroyed(); //&& mAnimation[getDirection()].isFinished();
}

bool PlayerEntity::giveKey()
{
    if(hasKey())
        return false;

    mHasKey = true;
    if(mUIKey)
        mUIKey->show();
    return true;
}

bool PlayerEntity::takeKey()
{
    if(!hasKey())
        return false;

    mHasKey = false;
    if(mUIKey)
        mUIKey->hide();
    return true;
}

bool PlayerEntity::hasKey() const
{
    return mHasKey;
}
