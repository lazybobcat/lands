#include "tilemap.h"
#include "../world.h"
#include <cassert>
#include "tinyXML/tinyxml2.h"
#include <iostream>

namespace xml = tinyxml2;

void TileMap::setTileset(sf::Texture *tileset)
{
    assert(tileset != nullptr);

    mTileset = tileset;
}


void TileMap::load(const std::string &file, World &world)
{
    assert(mTileset != nullptr);

    xml::XMLDocument doc;
    doc.LoadFile(file.c_str());
    xml::XMLElement *elem = doc.FirstChildElement("map");

    sf::Vector2u tileSize;
    unsigned int width = 0, height = 0;

    // Get general info from TMX file
    elem->QueryUnsignedAttribute("tilewidth", &tileSize.x);
    elem->QueryUnsignedAttribute("tilewidth", &tileSize.y);
    elem->QueryUnsignedAttribute("width", &width);
    elem->QueryUnsignedAttribute("height", &height);

    mVertices.clear();
    mVertices.setPrimitiveType(sf::Quads);
    mVertices.resize(width*height*4);

    world.setMapSize(sf::Vector2f(width*tileSize.x, height*tileSize.y));


    // TileMap generation
    elem = elem->FirstChildElement("layer");
    xml::XMLElement *tile = elem->FirstChildElement("data")->FirstChildElement();
    int tilenumber = 0;
    for(unsigned int j = 0; j < width; ++j)
    {
        for(unsigned int i = 0; i < height; ++i)
        {
            assert(tile);
            tile->QueryIntAttribute("gid", &tilenumber);

            int tu = (tilenumber-1) % (mTileset->getSize().x / tileSize.x);
            int tv = (tilenumber-1) / (mTileset->getSize().x / tileSize.x);

            // Pointer on current tile's quad
            sf::Vertex* quad = &mVertices[(i + j*width) * 4];

            // Corners
            quad[0].position = sf::Vector2f(i       * tileSize.x, j       * tileSize.y);
            quad[1].position = sf::Vector2f((i + 1) * tileSize.x, j       * tileSize.y);
            quad[2].position = sf::Vector2f((i + 1) * tileSize.x, (j + 1) * tileSize.y);
            quad[3].position = sf::Vector2f(i       * tileSize.x, (j + 1) * tileSize.y);

            // Define textures coords
            quad[0].texCoords = sf::Vector2f(tu       * tileSize.x, tv       * tileSize.y);
            quad[1].texCoords = sf::Vector2f((tu + 1) * tileSize.x, tv       * tileSize.y);
            quad[2].texCoords = sf::Vector2f((tu + 1) * tileSize.x, (tv + 1) * tileSize.y);
            quad[3].texCoords = sf::Vector2f(tu       * tileSize.x, (tv + 1) * tileSize.y);

            tile = tile->NextSiblingElement();
        }
    }

    // Objects
    while((elem = elem->NextSiblingElement()))
    {
        // Walls
        if(std::strcmp(elem->Attribute("name"), "walls") == 0)
        {
            xml::XMLElement *wall = elem->FirstChildElement();
            while(wall)
            {
                sf::Vector2f position, size;
                wall->QueryFloatAttribute("x", &position.x);
                wall->QueryFloatAttribute("y", &position.y);
                wall->QueryFloatAttribute("width", &size.x);
                wall->QueryFloatAttribute("height", &size.y);

                world.addWall(position, size);

                wall = wall->NextSiblingElement();
            }
        }

        // Ghosts Spawns
        else if(std::strcmp(elem->Attribute("name"), "ghostspawns") == 0)
        {
            xml::XMLElement *ghost = elem->FirstChildElement();
            while(ghost)
            {
                sf::Vector2f position;
                ghost->QueryFloatAttribute("x", &position.x);
                ghost->QueryFloatAttribute("y", &position.y);

                world.addGhost(position);

                ghost = ghost->NextSiblingElement();
            }
        }

        // Lights
        else if(std::strcmp(elem->Attribute("name"), "lights") == 0)
        {
            xml::XMLElement *light = elem->FirstChildElement();
            while(light)
            {
                sf::Vector2f position;
                float        ratio = 1.f;

                light->QueryFloatAttribute("x", &position.x);
                light->QueryFloatAttribute("y", &position.y);

                xml::XMLElement *property = light->FirstChildElement()->FirstChildElement();
                if(property)
                    property->QueryFloatAttribute("value", &ratio);
                else
                    std::cerr << "[TileMap] Enable to find ratio value for light !" << std::endl;

                world.addLight(position, ratio);

                light = light->NextSiblingElement();
            }
        }

        // Objects
        else if(std::strcmp(elem->Attribute("name"), "objects") == 0)
        {
            xml::XMLElement *obj = elem->FirstChildElement();
            while(obj)
            {
                sf::Vector2f position;
                unsigned int gid = 0;

                obj->QueryFloatAttribute("x", &position.x);
                obj->QueryFloatAttribute("y", &position.y);

                xml::XMLElement *property = obj->FirstChildElement();
                if(property && (property = property->FirstChildElement())) {
                    property->QueryUnsignedAttribute("value", &gid);
                } else {
                    std::cerr << "[TileMap] Unable to find id property of an object..." << std::endl;
                }

                world.addObject(position, gid);

                obj = obj->NextSiblingElement();
            }
        }

        // Player Spawn
        else if(std::strcmp(elem->Attribute("name"), "playerspawn") == 0)
        {
            xml::XMLElement *spawn = elem->FirstChildElement();
            sf::Vector2f position;

            spawn->QueryFloatAttribute("x", &position.x);
            spawn->QueryFloatAttribute("y", &position.y);

            world.setPlayerSpawn(position);
        }

        // Exit Spawn
        else if(std::strcmp(elem->Attribute("name"), "exit") == 0)
        {
            xml::XMLElement *spawn = elem->FirstChildElement();
            sf::Vector2f position, size;

            spawn->QueryFloatAttribute("x", &position.x);
            spawn->QueryFloatAttribute("y", &position.y);
            spawn->QueryFloatAttribute("width", &size.x);
            spawn->QueryFloatAttribute("height", &size.y);

            world.setExitPosition(position, size);
        }

        // Texts
        else if(std::strcmp(elem->Attribute("name"), "texts") == 0)
        {
            xml::XMLElement *text = elem->FirstChildElement();
            while(text)
            {
                sf::Vector2f position;
                std::string  content;

                text->QueryFloatAttribute("x", &position.x);
                text->QueryFloatAttribute("y", &position.y);

                xml::XMLElement *property = text->FirstChildElement();
                if(property && (property = property->FirstChildElement())) {
                    content = property->Attribute("value");
                } else {
                    std::cerr << "[TileMap] Unable to find content property of a text..." << std::endl;
                }

                world.addText(position, content);

                text = text->NextSiblingElement();
            }
        }
    }
}

void TileMap::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
    states.transform *= getTransform();

    states.texture = mTileset;

    target.draw(mVertices, states);
}

