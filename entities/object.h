#ifndef OBJECT_H
#define OBJECT_H

#include "scenenode.h"
#include "../resources/resourceholder.h"
#include "../resources/resourceidentifiers.h"

class Object : public SceneNode
{
public:
    enum Type
    {
        Table,
        Slime,
        Shit,
        Mushroom,
        Shrooms,
        TreeNW,
        TreeNE,
        TreeSW,
        TreeSE,
        Key,
        DoorTop,
        DoorBottom,
        TypeCount
    };

public:
    Object(Type type, TextureHolder& textures);

    virtual unsigned int    getCategory() const;
    virtual sf::FloatRect   getBoundingRect() const;

    virtual bool            isCollidable() const;
    virtual bool            isDestroyed() const;

    void                    destroy();

private:
    virtual void            drawCurrent(sf::RenderTarget &target, sf::RenderStates states) const;
    //virtual void            updateCurrent(sf::Time dt, CommandQueue &commands);

private:
    sf::Sprite      mSprite;
    bool            mIsCollidable = false;
    Type            mType;
    Category::Type  mCategory = Category::Decoration;
    bool            mIsDestroyed = false;
};

#endif // OBJECT_H
