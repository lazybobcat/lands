#ifndef RESOURCE_IDENTIFIERS_H
#define RESOURCE_IDENTIFIERS_H

// Forward declarations
namespace sf
{
    class Texture;
    class Font;
    class Shader;
    class Music;
    class SoundBuffer;
}

template <typename Resource, typename Identifier>
class ResourceHolder;



// Textures
namespace Textures
{
    enum ID
    {
        Particle,
        Player,
        PlayerWalkingDown,
        PlayerWalkingUp,
        PlayerWalkingLeft,
        PlayerWalkingRight,
        PlayerDead,
        Ghost,
        GhostDead,
        Halo,
        LaserBullet,
        Wall,
        Tileset,
        Objects,
        UI
    };
}

typedef ResourceHolder<sf::Texture, Textures::ID> TextureHolder;


// Fonts
namespace Fonts
{
    enum ID
    {
        Main,
        Flipps
    };
}

typedef ResourceHolder<sf::Font, Fonts::ID> FontHolder;


// Shaders
namespace Shaders
{
    enum ID
    {

    };
}
typedef ResourceHolder<sf::Shader, Shaders::ID> ShaderHolder;


// Musics
namespace Musics
{
    enum ID
    {
        TitleScreen,
        MainTheme
    };
}


// Sounds
namespace Sounds
{
    enum ID
    {
        Shot,
        GhostHunting,
        GhostDying,
        GhostHurt,
        PlayerDying,
        PlayerHurt,
        PlayerSpawning,
        PlayerExiting,
        PickupKey,
        OpenDoor
    };
}
typedef ResourceHolder<sf::SoundBuffer, Sounds::ID> SoundHolder;


// Maps
namespace Maps
{
    enum ID
    {
        Tutorial = 0,
        Level1,
        Level2,

        EndOfMapList
    };
}


// Scripts
namespace Scripts
{
    enum ID
    {
        Loading,
        Load
    };
}



#endif // RESOURCE_IDENTIFIERS_H
