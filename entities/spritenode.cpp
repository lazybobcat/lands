#include "spritenode.h"

SpriteNode::SpriteNode() {

}


SpriteNode::SpriteNode(const sf::Texture &texture) :
    mSprite(texture)
{
}

SpriteNode::SpriteNode(const sf::Texture &texture, const sf::IntRect& rect) :
    mSprite(texture, rect)
{
}


void SpriteNode::setTexture(const sf::Texture &texture)
{
    mSprite.setTexture(texture);
}

void SpriteNode::drawCurrent(sf::RenderTarget &target, sf::RenderStates states) const
{
    target.draw(mSprite, states);
}
