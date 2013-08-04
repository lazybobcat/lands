#include "healthbar.h"

Healthbar::Healthbar(sf::Vector2f size) :
    SceneNode(),
    mPercent(1.f),
    mSize(size)
{
}


bool Healthbar::isCollidable() const
{
    return false;
}

void Healthbar::drawCurrent(sf::RenderTarget &target, sf::RenderStates states) const
{
    sf::RectangleShape green, red;
    green.setFillColor(sf::Color(135,215,105));
    green.setSize(sf::Vector2f(mSize.x * mPercent, mSize.y));
    green.setOrigin(sf::Vector2f(mSize.x/2, mSize.y/2));
    red.setFillColor(sf::Color(205,40,60));
    red.setSize(mSize);
    red.setOrigin(sf::Vector2f(mSize.x/2, mSize.y/2));

    target.draw(red, states);
    target.draw(green, states);
}

void Healthbar::setPercentage(float prct)
{
    mPercent = prct;
}

