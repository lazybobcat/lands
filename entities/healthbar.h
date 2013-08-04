#ifndef HEALTHBAR_H
#define HEALTHBAR_H

#include "scenenode.h"
#include "../resources/resourceholder.h"
#include "../resources/resourceidentifiers.h"

#include <SFML/Graphics.hpp>

class Healthbar : public SceneNode
{
public:
    explicit Healthbar(sf::Vector2f size = sf::Vector2f(60, 10));

    virtual bool    isCollidable() const;
    virtual void    drawCurrent(sf::RenderTarget &target, sf::RenderStates states) const;

    void            setPercentage(float prct);

private:
    float           mPercent;
    sf::Vector2f    mSize;
};

#endif // HEALTHBAR_H
