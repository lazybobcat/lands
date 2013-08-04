#ifndef PLAYERENTITY_H
#define PLAYERENTITY_H

#include "entity.h"
#include "healthbar.h"
#include "uisprite.h"
#include "../resources/resourceholder.h"
#include "../resources/resourceidentifiers.h"
#include "../resources/animation.h"

#include <array>

class PlayerEntity : public Entity
{
public:
    typedef std::unique_ptr<PlayerEntity> Ptr;

public:
    PlayerEntity(TextureHolder& textures);

    virtual unsigned int    getCategory() const;

    virtual bool            isAllied() const;

    virtual sf::FloatRect   getBoundingRect() const;
    virtual bool            isCollidable() const;
    virtual bool            isMarkedForRemoval() const;

    bool                    giveKey();
    bool                    takeKey();
    bool                    hasKey() const;


private:
    virtual void drawCurrent(sf::RenderTarget &target, sf::RenderStates states) const;
    virtual void updateCurrent(sf::Time dt, CommandQueue &commands);
    void         updateAnimation(sf::Time dt);
    void         updateDirection();

private:
    sf::Sprite      mSprite;
    std::array<Animation, 4> mAnimations;
    Animation       mDieAnimation;

    Command         mFireCommand;
    bool            mIsFiring = false;
    Healthbar*      mHealthbar;
    UISprite*       mUIKey;
    Command         mAddUiCommand;

    bool            mMovingAnimation = false;

    bool            mHasKey = false;
};

#endif // PLAYERENTITY_H
