#include "world.h"
#include "utils.h"
#include <algorithm>

#include <iostream>

////////////////////////////////////////////////////

bool matchesCatetegories(SceneNode::Pair& colliders, Category::Type type1, Category::Type type2)
{
    unsigned int category1 = colliders.first->getCategory();
    unsigned int category2 = colliders.second->getCategory();

    if(type1 & category1 && type2 & category2)
    {
        return true;
    }
    else if(type1 & category2 && type2 & category1)
    {
        std::swap(colliders.first, colliders.second);
        return true;
    }

    return false;
}

std::string getMapFile(Maps::ID map)
{
    switch(map)
    {
        case Maps::Level1:
        return "assets/maps/level1.tmx";

        case Maps::Level2:
        return "assets/maps/level2.tmx";

        case Maps::Tutorial:
        default:
        return "assets/maps/tuto.tmx";
    }
}

Maps::ID    ActualLevel = Maps::Tutorial;
bool        AlreadyLoaded = false;

////////////////////////////////////////////////////

World* World::mInstance = nullptr;

World* World::instance()
{
    return World::mInstance;
}

////////////////////////////////////////////////////

World::World(sf::RenderWindow &window, TextureHolder &textures, FontHolder &fonts, SoundPlayer& sounds, ScriptPlayer& scripts, PlayerController& pcontroller) :
    mWindow(window),
    mWorldView(window.getDefaultView()),
    mFrontView(sf::Vector2f(0.f, 0.f), sf::Vector2f(window.getSize().x, window.getSize().y)),
    mUIView(sf::Vector2f(0.f, 0.f), sf::Vector2f(window.getSize().x, window.getSize().y)),
    mTextures(textures),
    mFonts(fonts),
    mSounds(sounds),
    mScripts(scripts),
    mSceneGraph(),
    mQuadTree(1, sf::FloatRect(0.f, 0.f, window.getSize().x, window.getSize().y)),
    mSceneLayers(),
    mPlayerController(pcontroller),
    // Nodes
    mPlayer(nullptr)
{
    mSceneTexture.create(mWindow.getSize().x, mWindow.getSize().y);
    mLightTexture.create(mWindow.getSize().x, mWindow.getSize().y);

    mFrontView.setCenter(sf::Vector2f(window.getSize().x / 2.f, window.getSize().y / 2.f));
    mUIView.setCenter(sf::Vector2f(window.getSize().x / 2.f, window.getSize().y / 2.f));

    mMapScale = sf::Vector2f(2.f, 2.f);

    //
    World::mInstance = this;
    //


    loadTextures();
    buildScene();

    // Sounds
    std::unique_ptr<SoundNode> sound(new SoundNode(sounds));
    mSceneGraph.attachChild(std::move(sound));


    // Other things here, like setting the view center on the player, scores, etc...
}


void World::loadTextures()
{
    if(AlreadyLoaded)
        return;

    mTextures.load(Textures::Player, "assets/textures/player_standing.png");
    mTextures.load(Textures::PlayerWalkingDown, "assets/textures/player_walking_down.png");
    mTextures.load(Textures::PlayerWalkingLeft, "assets/textures/player_walking_left.png");
    mTextures.load(Textures::PlayerWalkingRight, "assets/textures/player_walking_right.png");
    mTextures.load(Textures::PlayerWalkingUp, "assets/textures/player_walking_up.png");
    mTextures.load(Textures::PlayerDead, "assets/textures/player_dying.png");
    mTextures.load(Textures::Ghost, "assets/textures/ghost_stands.png");
    mTextures.load(Textures::GhostDead, "assets/textures/ghost_dying.png");
    mTextures.load(Textures::Halo, "assets/textures/halo.png");
    mTextures.load(Textures::LaserBullet, "assets/textures/laser.png");
    mTextures.load(Textures::Tileset, "assets/textures/tiles.png");
    mTextures.load(Textures::Objects, "assets/textures/objects.png");
    mTextures.load(Textures::UI, "assets/textures/ui.png");

    AlreadyLoaded = true;
}

void World::buildScene()
{
    // Lua loading script
    mScripts.registerFile(Scripts::Loading, "assets/scripts/loading.lua");

    // Launch lua scripts
    mScripts.play(Scripts::Loading);



    // Initialize layers
    for(std::size_t i = 0; i < LayerCount; ++i)
    {
        Category::Type cat = ((i == Entities) ? Category::EntityLayer : ((i == Lights) ? Category::LightLayer : ((i == Foreground) ? Category::UILayer : Category::None)));
        SceneNode::Ptr layer(new SceneNode(cat));
        mSceneLayers[i] = layer.get();
        mSceneGraph.attachChild(std::move(layer));
    }



    // Add particle node to the scene
    // Player
    PlayerEntity::Ptr player(new PlayerEntity(mTextures));
    player->setPosition(100.f, 100.f);
    mPlayer = player.get();

    // Lights
    mHalo.setTexture(mTextures.get(Textures::Halo));
    centerOrigin(mHalo);
    mHalo.setScale(sf::Vector2f(2.f, 2.f));
    mHalo.setPosition(sf::Vector2f(mWindow.getSize().x / 2.f, mWindow.getSize().y / 2.f));

    mLightSprite.setTexture(mTextures.get(Textures::Halo));
    centerOrigin(mLightSprite);


    // Tiled map
    if(ActualLevel == Maps::Tutorial && mSkipTutorial)
        ActualLevel = static_cast<Maps::ID>(ActualLevel+1);

    if(mSetLevel < Maps::EndOfMapList)
        ActualLevel = static_cast<Maps::ID>(mSetLevel);

    loadMap(getMapFile(ActualLevel));


    // Attach player to scene
    mSceneLayers[Layer::Entities]->attachChild(std::move(player));

    // Spawn Sound !
    mSounds.play(Sounds::PlayerSpawning);
}

void World::update(sf::Time dt)
{
    /// Game logic here
    //mHalo.setPosition(mPlayer->getWorldPosition());
    mWorldView.setCenter(mPlayer->getWorldPosition());


    // Remove useless entities
    destroyEntitiesOutsideView();

    // Update quadtree
    mQuadTree.clear();
    mCollidableNodes.clear();
    Command command;
    command.category = Category::All;
    command.action = derivedAction<SceneNode>([this] (SceneNode& node, sf::Time) {
        mQuadTree.insert(&node);
        if(node.isCollidable())
            mCollidableNodes.push_back(&node);
    });
    mCommandQueue.push(command);

    // Forward commands to scene
    while(!mCommandQueue.isEmpty())
    {
        mSceneGraph.onCommand(mCommandQueue.pop(), dt);
    }

    // Handle collisions
    handleCollisions();

    // End game conditions
    if(mPlayer->isDestroyed())
        return;

    // Destroy dead entities
    mSceneGraph.removeWrecks();

    // Set the listener position
    mSounds.setListenerPosition(mPlayer->getWorldPosition());
    mSounds.removeStoppedSounds();

    mSceneGraph.update(dt, mCommandQueue);
}


void World::handleCollisions()
{

    /*mSceneGraph.checkSceneCollision(mSceneGraph, collisionPairs);*/
    std::deque<SceneNode*> proxim;
    std::set<SceneNode::Pair> checked;
    for(SceneNode* node1 : mCollidableNodes)
    {
        proxim.clear();
        mQuadTree.getCloseObjects(node1, proxim);
        // Check proxim collisions here
        for(SceneNode* node2 : proxim)
        {
            if(node1 == node2)
                continue;

            if(!collision(*node1, *node2))
                continue;

            SceneNode::Pair pair(std::minmax(node1, node2));
            if(checked.find(pair) != checked.end())
                continue;

            checked.insert(pair);

            if(matchesCatetegories(pair, Category::Player, Category::Ghost))
            {
                auto& player = static_cast<PlayerEntity&>(*pair.first);
                auto& ghost = static_cast<GhostEntity&>(*pair.second);

                // Collision player<>ghost : player is damaged
                // CHEAAAAAT
                if(!mPlayerInvincible)
                {
                    player.damage(ghost.getDamages());
                }

                ghost.destroy();
                mSounds.play(Sounds::GhostDying, ghost.getWorldPosition());
                if(player.isDestroyed())
                    mSounds.play(Sounds::PlayerDying, player.getWorldPosition());
                else
                    mSounds.play(Sounds::PlayerHurt, player.getWorldPosition());
            }
            else if(matchesCatetegories(pair, Category::Ghost, Category::AlliedBullet))
            {
                auto& ghost = static_cast<GhostEntity&>(*pair.first);
                auto& bullet = static_cast<Projectile&>(*pair.second);

                // Collision ghost<>buller : ghost is damaged
                if(ghost.isDestroyed())
                    return;

                ghost.damage(bullet.getDamages());
                if(ghost.isDestroyed())
                    mSounds.play(Sounds::GhostDying, ghost.getWorldPosition());
                else
                    mSounds.play(Sounds::GhostHurt, ghost.getWorldPosition());
                bullet.destroy();
            }
            else if(matchesCatetegories(pair, Category::Player, Category::Wall))
            {
                auto& player = static_cast<PlayerEntity&>(*pair.first);
                //auto& wall = static_cast<Wall&>(*pair.second);

                // Collision player<>wall : Should not get through

                // That's not a very good collision handling, but It'll do it for now
                player.stop();
                player.demove();
            }
            else if(matchesCatetegories(pair, Category::Player, Category::Decoration))
            {
                auto& player = static_cast<PlayerEntity&>(*pair.first);
                //auto& wall = static_cast<Wall&>(*pair.second);

                // Collision player<>wall : Should not get through

                // That's not a very good collision handling, but It'll do it for now
                player.stop();
                player.demove();
            }
            else if(matchesCatetegories(pair, Category::AlliedBullet, Category::Wall))
            {
                auto& bullet = static_cast<Projectile&>(*pair.first);
                bullet.destroy();
            }
            else if(matchesCatetegories(pair, Category::Player, Category::Key))
            {
                auto& key = static_cast<Object&>(*pair.second);

                if(mPlayer->giveKey()) {
                    key.destroy();
                    mSounds.play(Sounds::PickupKey);
                }
            }
            else if(matchesCatetegories(pair, Category::Player, Category::Door))
            {
                auto& player = static_cast<PlayerEntity&>(*pair.first);
                auto& door = static_cast<Object&>(*pair.second);

                if(mPlayer->hasKey())
                {
                    door.destroy();
                    mSounds.play(Sounds::OpenDoor);
                }
                else
                {
                    // Collision player<>wall : Should not get through
                    // That's not a very good collision handling, but It'll do it for now
                    player.stop();
                    player.demove();
                }
            }
            else if(matchesCatetegories(pair, Category::Player, Category::Exit))
            {
                mPlayerController.mMissionStatus = PlayerController::Success;
                mSounds.play(Sounds::PlayerExiting);
                // won <3
            }
        }
    }
}


void World::drawLights()
{
    //return;
    mLightTexture.clear(sf::Color(0,0,0));
    mLightTexture.setView(mFrontView);
    mLightTexture.draw(mHalo);

    mLightTexture.setView(mWorldView);
    for(Light& l : mLights)
    {
        mLightSprite.setPosition(l.position);
        mLightSprite.setColor(l.color);
        mLightSprite.setScale(sf::Vector2f(l.ratio, l.ratio));
        mLightTexture.draw(mLightSprite);
    }

    mLightTexture.display();
}

void World::draw()
{
    mWindow.clear(sf::Color::Blue);

    mSceneTexture.clear(sf::Color(0,0,0));
    mSceneTexture.setView(mWorldView);
    mSceneTexture.draw(mTileMap);
    for(int i = 0; i < Foreground; ++i)
    {
        mSceneTexture.draw(*mSceneLayers[i]);
    }
    //mQuadTree.draw(mSceneTexture);

    // Lightning ...?
    drawLights();

    sf::Sprite prerendering(mLightTexture.getTexture());
    mSceneTexture.setView(mFrontView);
    mSceneTexture.draw(prerendering, sf::BlendMultiply);
    mSceneTexture.display(); //Better...

    sf::Sprite scene(mSceneTexture.getTexture());

    //mWindow.setView(mFrontView);
    //mWindow.draw(mTileMap);
    mWindow.draw(scene);
    mWindow.setView(mWindow.getDefaultView());
    mWindow.draw(*mSceneLayers[Foreground]);
}


void World::destroyEntitiesOutsideView()
{
    Command command;
    command.category = Category::AlliedBullet | Category::EnemyBullet;
    command.action = derivedAction<Entity>([this] (Entity& e, sf::Time) {
        sf::FloatRect bounds = getViewBounds();
        if(!bounds.intersects(e.getBoundingRect()))
        {
            e.destroy();
        }
    });
    // IM SO FUCKING STUPID
    mCommandQueue.push(command);
}

sf::FloatRect World::getViewBounds() const
{
    return sf::FloatRect(mWorldView.getCenter() - mWorldView.getSize() / 2.f, mWorldView.getSize());
}


bool World::hasPlayerAlive() const
{
    return !mPlayer->isDestroyed();
}


CommandQueue& World::getCommandQueue()
{
    return mCommandQueue;
}

////////////////////////////////////////////////////



void World::loadMap(const std::string &file)
{
    mTileMap.setTileset(&mTextures.get(Textures::Tileset));
    mTileMap.load(file, *this);
    mTileMap.setScale(mMapScale);
}

void World::addWall(sf::Vector2f position, sf::Vector2f size)
{
    position.x = position.x * mMapScale.x;
    position.y = position.y * mMapScale.y;
    size.x = size.x * mMapScale.x;
    size.y = size.y * mMapScale.y;

    std::unique_ptr<Wall> wall(new Wall(sf::FloatRect(position, size)));
    mSceneLayers[Layer::Entities]->attachChild(std::move(wall));
}

void World::addGhost(sf::Vector2f position)
{
    position.x = position.x * mMapScale.x;
    position.y = position.y * mMapScale.y;

    GhostEntity::Ptr enemy(new GhostEntity(mGhostController, mTextures));
    enemy->setPosition(position);
    mSceneLayers[Layer::Entities]->attachChild(std::move(enemy));
}

void World::setPlayerSpawn(sf::Vector2f position)
{
    assert(mPlayer);
    position.x = position.x * mMapScale.x;
    position.y = position.y * mMapScale.y;

    mPlayer->setPosition(position);
}

void World::setExitPosition(sf::Vector2f position, sf::Vector2f size)
{
    // todo
    position.x = position.x * mMapScale.x;
    position.y = position.y * mMapScale.y;
    size.x = size.x * mMapScale.x;
    size.y = size.y * mMapScale.y;

    std::unique_ptr<Exit> exit(new Exit(sf::FloatRect(position, size)));
    mSceneGraph.attachChild(std::move(exit));
}

void World::setMapSize(sf::Vector2f size)
{
    size.x = size.x * mMapScale.x;
    size.y = size.y * mMapScale.y;

    mQuadTree.setBounds(sf::FloatRect(0.f, 0.f, size.x, size.y));
}


void World::addLight(sf::Vector2f position, float radiusRatio)
{
    position.x = position.x * mMapScale.x;
    position.y = position.y * mMapScale.y;
    mLights.push_back(Light(position, radiusRatio));
}

void World::addObject(sf::Vector2f position, unsigned int id)
{
    position.x = position.x * mMapScale.x;
    position.y = position.y * mMapScale.y;

    Object::Type type = Object::Shrooms;

    switch(id)
    {
        case 1:
            type = Object::Table;
            break;

        case 2:
            type = Object::DoorTop;
            break;

        case 3:
            type = Object::Shit;
            break;

        case 4:
            type = Object::Key;
            break;

        case 5:
            type = Object::TreeNW;
            break;

        case 6:
            type = Object::TreeNE;
            break;

        case 7:
            type = Object::Slime;
            break;

        case 8:
            type = Object::DoorBottom;
            break;

        case 9:
            type = Object::Mushroom;
            break;

        case 10:
            type = Object::Shrooms;
            break;

        case 11:
            type = Object::TreeSW;
            break;

        case 12:
            type = Object::TreeSE;
            break;

        default:
            std::cout << "Unhandled Object #" << id << std::endl;
            return;

    }

    std::unique_ptr<Object> obj(new Object(type, mTextures));
    obj->setPosition(position);
    obj->setScale(mMapScale);
    mSceneLayers[Layer::Background]->attachChild(std::move(obj));
}

void World::addText(sf::Vector2f position, const std::string &text)
{
    position.x = position.x * mMapScale.x;
    position.y = position.y * mMapScale.y;

    std::unique_ptr<TextNode> obj(new TextNode(text, mFonts));
    obj->setPosition(position);
    mSceneLayers[Layer::Background]->attachChild(std::move(obj));
}



////////////////////////////////////////////////////

void World::setPlayerInvincible(bool flag)
{
    mPlayerInvincible = flag;
}

void World::skipTutorial()
{
    mSkipTutorial = true;
    std::cout << "Tutorial Level Skipped !" << std::endl;
}

void World::setLevel(int level)
{
    if(level < (int)Maps::EndOfMapList && level >= 0)
    {
        mSetLevel = level;

        std::cout << "Game will only play level nb " << level << std::endl;
    }
}


