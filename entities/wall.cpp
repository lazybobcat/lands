#include "wall.h"
#include "../resources/resourceholder.h"
#include "../utils.h"

Wall::Wall(const sf::FloatRect &bounds) :
    SceneNode(Category::Wall),
    mBounds(bounds)
{
}


void Wall::drawCurrent(sf::RenderTarget &target, sf::RenderStates states) const
{
#ifdef DEBUG
    sf::RectangleShape wall;
    wall.setFillColor(sf::Color(255, 0, 0, 150));
    wall.setPosition(mBounds.left, mBounds.top);
    wall.setSize(sf::Vector2f(mBounds.width, mBounds.height));
    target.draw(wall, states);
#endif
}


bool Wall::isCollidable() const
{
    return true;
}

sf::FloatRect Wall::getBoundingRect() const
{
    return mBounds;
}

unsigned int Wall::getCategory() const
{
    return Category::Wall;
}

