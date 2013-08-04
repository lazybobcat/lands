#ifndef UIKEY_H
#define UIKEY_H

#include "scenenode.h"
#include "../resources/resourceholder.h"
#include "../resources/resourceidentifiers.h"
#include <SFML/Graphics.hpp>

class UISprite : public SceneNode
{
public:
    UISprite(TextureHolder& resources, Textures::ID id, const sf::IntRect& textureRect = sf::IntRect());

    void    show();
    void    hide();

private:
    virtual void drawCurrent(sf::RenderTarget &target, sf::RenderStates states) const;

private:
    sf::Sprite  mSprite;
    bool        mShow = false;
};

#endif // UIKEY_H
