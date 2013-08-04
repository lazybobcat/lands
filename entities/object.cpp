#include "object.h"
#include "../datatables.h"
#include "../utils.h"

namespace
{
    std::vector<ObjectData> Data = initializeObjectData();
}

Object::Object(Type type, TextureHolder &textures) :
    SceneNode(Category::Decoration),
    mType(type)
{
    mSprite.setTexture(textures.get(Data[mType].texture));
    mSprite.setTextureRect(Data[mType].textureRect);
    centerOrigin(mSprite);
    mSprite.setPosition(mSprite.getPosition() - sf::Vector2f(-mSprite.getLocalBounds().width / 2, mSprite.getLocalBounds().height / 2));

    mIsCollidable = Data[mType].isCollidable;
    mCategory = Data[mType].category;
}


void Object::drawCurrent(sf::RenderTarget &target, sf::RenderStates states) const
{
    if(!isDestroyed())
        target.draw(mSprite, states);
}


unsigned int Object::getCategory() const
{
    return mCategory;
}

sf::FloatRect Object::getBoundingRect() const
{
    return getWorldTransform().transformRect(mSprite.getGlobalBounds());
}

bool Object::isCollidable() const
{
    return mIsCollidable;
}
bool Object::isDestroyed() const
{
    return mIsDestroyed;
}

void Object::destroy()
{
    mIsDestroyed = true;
}
