#include <application.h>

#include "tinyXML/tinyxml2.h"
#include <iostream>

namespace xml = tinyxml2;

int main()
{
    const unsigned int width = 1024, height = width * 3/4;


    /*{
        xml::XMLDocument doc;
        doc.LoadFile("assets/maps/lands.tmx");
        xml::XMLElement *elem = doc.FirstChildElement("map");
        unsigned int width = 0, height = 0, nbTiles = 0, nbWalls = 0, nbGhosts = 0;

        elem->QueryUnsignedAttribute("width", &width);
        elem->QueryUnsignedAttribute("height", &height);

        elem = elem->FirstChildElement("layer");
        xml::XMLElement *tile = elem->FirstChildElement("data")->FirstChildElement();
        while(tile != nullptr)
        {
            nbTiles++;
            tile = tile->NextSiblingElement();
        }

        while((elem = elem->NextSiblingElement()))
        {
            if(std::strcmp(elem->Attribute("name"), "walls") == 0)
            {
                xml::XMLElement *walls = elem->FirstChildElement();
                while(walls)
                {
                    nbWalls++;
                    walls = walls->NextSiblingElement();
                }
            }
            else if(std::strcmp(elem->Attribute("name"), "ghostspawns") == 0)
            {
                xml::XMLElement *ghost = elem->FirstChildElement();
                while(ghost)
                {
                    nbGhosts++;
                    ghost = ghost->NextSiblingElement();
                }
            }
        }


        std::cout << "La map fait " << width << "x" << height << " tiles !" << std::endl;
        std::cout << "Elle comporte " << nbTiles << " tiles." << std::endl;
        std::cout << "Il y a " << nbWalls << " murs !" << std::endl;
        std::cout << "Attention, il y a " << nbGhosts << " fantomes !" << std::endl;
    }*/




    Application game(width, height, "LANDS");
    game.run();
    return 0;
}

