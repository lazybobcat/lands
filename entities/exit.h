#ifndef EXIT_H
#define EXIT_H

#include "scenenode.h"

#include <SFML/Graphics/Rect.hpp>

class Exit : public SceneNode
{
public:
    explicit Exit(const sf::FloatRect& bounds);

    virtual sf::FloatRect   getBoundingRect() const;
    virtual bool            isCollidable() const;
    virtual unsigned int    getCategory() const;

private:
    sf::FloatRect   mBounds;
};

#endif // EXIT_H
