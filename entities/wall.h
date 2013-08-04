#ifndef WALL_H
#define WALL_H

#include "scenenode.h"
#include "../resources/resourceidentifiers.h"

class Wall : public SceneNode
{
public:
    Wall(const sf::FloatRect& bounds);

    virtual bool            isCollidable() const;

    virtual sf::FloatRect   getBoundingRect() const;
    virtual unsigned int    getCategory() const;

private:
    virtual void drawCurrent(sf::RenderTarget &target, sf::RenderStates states) const;

private:
    sf::FloatRect           mBounds;
};

#endif // WALL_H
