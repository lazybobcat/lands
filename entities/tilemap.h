#ifndef TILEMAP_H
#define TILEMAP_H

#include "../resources/resourceidentifiers.h"
#include "../resources/resourceholder.h"

#include <SFML/Graphics.hpp>

class World;

class TileMap : public sf::Drawable, public sf::Transformable
{
public:
    void            setTileset(sf::Texture* tileset);
    void            load(const std::string& file, World& world);
    //void            load(sf::Vector2u tileSize, const int* tiles, unsigned int width, unsigned int height);

private:
    virtual void    draw(sf::RenderTarget &target, sf::RenderStates states) const;

private:
    sf::VertexArray mVertices;
    sf::Texture*    mTileset = nullptr;
};

#endif // TILEMAP_H
