#ifndef SPRITENODE_H
#define SPRITENODE_H

#include "scenenode.h"

class SpriteNode : public SceneNode
{
public:
    SpriteNode();
    explicit SpriteNode(const sf::Texture& texture);
    SpriteNode(const sf::Texture &texture, const sf::IntRect& rect);

   void    setTexture(const sf::Texture& texture);

private:
    virtual void    drawCurrent(sf::RenderTarget &target, sf::RenderStates states) const;

private:
    sf::Sprite      mSprite;
};

#endif // SPRITENODE_H
