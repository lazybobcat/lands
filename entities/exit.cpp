#include "exit.h"

Exit::Exit(const sf::FloatRect &bounds) :
    SceneNode(Category::Exit),
    mBounds(bounds)
{
}


sf::FloatRect Exit::getBoundingRect() const
{
    return mBounds;
}

bool Exit::isCollidable() const
{
    return true;
}

unsigned int Exit::getCategory() const
{
    return Category::Exit;
}
