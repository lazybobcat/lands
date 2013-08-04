#include "uisprite.h"
#include "../utils.h"

UISprite::UISprite(TextureHolder &resources, Textures::ID id, const sf::IntRect& textureRect) :
    SceneNode()
{
    mSprite.setTexture(resources.get(id));
    mSprite.setTextureRect(textureRect);
    mSprite.setScale(sf::Vector2f(2.f, 2.f));
    centerOrigin(mSprite);
}


void UISprite::show()
{
    mShow = true;
}

void UISprite::hide()
{
    mShow = false;
}

void UISprite::drawCurrent(sf::RenderTarget &target, sf::RenderStates states) const
{
    if(mShow)
    {
        states.transform = getWorldTransform();
        target.draw(mSprite, states); // IDIOT MAN IS MY NAME ........ !!!!!
    }
}
