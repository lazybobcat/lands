#ifndef GHOSTENTITY_H
#define GHOSTENTITY_H

#include "entity.h"
#include "../resources/resourceholder.h"
#include "../resources/resourceidentifiers.h"
#include "./resources/animation.h"
#include "../controllers/ghostcontroller.h"

class GhostEntity : public Entity
{
public:
    typedef std::unique_ptr<GhostEntity> Ptr;

public:
    GhostEntity(GhostController& controller, TextureHolder& textures);

    virtual unsigned int    getCategory() const;

    virtual sf::FloatRect   getBoundingRect() const;
    virtual bool            isCollidable() const;
    void                    updateDirection();

    virtual bool            isMarkedForRemoval() const;

private:
    virtual void drawCurrent(sf::RenderTarget &target, sf::RenderStates states) const;
    virtual void updateCurrent(sf::Time dt, CommandQueue &commands);

private:
    friend class GhostController;

    GhostController& mController;

    sf::Sprite      mSprite;
    Animation       mDieAnimation;
    GhostController::Action mAction = GhostController::Hunt;
    sf::Time        mRestTimer = sf::Time::Zero;

};

#endif // GHOSTENTITY_H
