#ifndef WORLD_H
#define WORLD_H

#include <array>
#include <list>

#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>

#include "resources/resourceholder.h"
#include "resources/resourceidentifiers.h"
#include "resources/scriptplayer.h"
#include "events/category.h"
#include "events/commandqueue.h"
#include "entities/playerentity.h"
#include "entities/ghostentity.h"
#include "entities/projectile.h"
#include "entities/wall.h"
#include "entities/quadtree.h"
#include "entities/tilemap.h"
#include "entities/exit.h"
#include "entities/object.h"
#include "entities/textnode.h"
#include "controllers/ghostcontroller.h"
#include "controllers/playercontroller.h"
#include "light.h"


extern Maps::ID ActualLevel;


class World : public sf::NonCopyable
{
public:
    World(sf::RenderWindow& window, TextureHolder &textures, FontHolder &fonts, SoundPlayer& sounds, ScriptPlayer& scripts, PlayerController& pcontroller);

    static World*   instance();

    void        update(sf::Time dt);
    void        draw();

    CommandQueue&   getCommandQueue();

    bool        hasPlayerAlive() const;

    //// Map Construction ////
    void        loadMap(const std::string& file);
    void        addWall(sf::Vector2f position, sf::Vector2f size);
    void        addGhost(sf::Vector2f position);
    void        setPlayerSpawn(sf::Vector2f position);
    void        setExitPosition(sf::Vector2f position, sf::Vector2f size);
    void        setMapSize(sf::Vector2f size);
    void        addLight(sf::Vector2f position, float radiusRatio);
    void        addObject(sf::Vector2f position, unsigned int id);
    void        addText(sf::Vector2f position, const std::string& text);

    //// Cheats ////
    void        setPlayerInvincible(bool flag = true);
    void        skipTutorial();
    void        setLevel(int level);


private:
    void        loadTextures();
    void        buildScene();

    void        handleCollisions();

    void        drawLights();

    void        destroyEntitiesOutsideView();
    sf::FloatRect getViewBounds() const;


private:
    enum Layer
    {
        Background,
        Entities,
        Lights,
        Foreground,
        LayerCount
    };

private:
    static World*   mInstance;

    sf::RenderWindow&                   mWindow;
    sf::RenderTexture                   mSceneTexture; // Use RenderTexture instead of window to drawto apply some shaders or pre-redering...
    sf::RenderTexture                   mLightTexture;
    sf::View                            mWorldView;
    sf::View                            mFrontView;
    sf::View                            mUIView;
    TextureHolder&                      mTextures;
    FontHolder&                         mFonts;
    SoundPlayer&                        mSounds;
    ScriptPlayer&                       mScripts;

    SceneNode                           mSceneGraph;
    QuadTree                            mQuadTree;
    std::array<SceneNode*,LayerCount>   mSceneLayers;
    CommandQueue                        mCommandQueue;

    // World
    TileMap                             mTileMap;
    sf::Vector2f                        mMapScale;
    std::list<SceneNode*>               mCollidableNodes;

    // Player
    PlayerController&                   mPlayerController;
    PlayerEntity*                       mPlayer;
    sf::Sprite                          mHalo;

    // Enemies
    GhostController                     mGhostController;

    // Lights
    sf::Sprite                          mLightSprite;
    std::list<Light>                    mLights;

    //// Cheats ////
    bool                        mPlayerInvincible = false;
    bool                        mSkipTutorial = false;
    int                         mSetLevel = (int)Maps::EndOfMapList;
};

#endif // WORLD_H
