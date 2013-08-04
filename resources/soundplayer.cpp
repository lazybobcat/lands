#include "soundplayer.h"
#include "../utils.h"

namespace
{
    const float ListenerZ = 200.f;
    const float Attenuation = 5.f;
    const float MinDistance2D = 200.f;
    const float MinDistance3D = std::sqrt(MinDistance2D*MinDistance2D + ListenerZ*ListenerZ);
}

/////////////////////////////////////////

SoundPlayer::SoundPlayer() :
    mSoundBuffers(),
    mSounds()
{
    // Fill buffers with sounds
    mSoundBuffers.load(Sounds::Shot, "assets/sounds/shot.wav");
    mSoundBuffers.load(Sounds::GhostHunting, "assets/sounds/ghost_hunting.wav");
    mSoundBuffers.load(Sounds::GhostDying, "assets/sounds/ghost_dying.wav");
    mSoundBuffers.load(Sounds::GhostHurt, "assets/sounds/ghost_hurt.wav");
    mSoundBuffers.load(Sounds::PlayerDying, "assets/sounds/player_dying.wav");
    mSoundBuffers.load(Sounds::PlayerHurt, "assets/sounds/player_hurt.wav");
    mSoundBuffers.load(Sounds::PlayerSpawning, "assets/sounds/player_spawning.wav");
    mSoundBuffers.load(Sounds::PlayerExiting, "assets/sounds/player_exiting.wav");
    mSoundBuffers.load(Sounds::PickupKey, "assets/sounds/pickup_key.wav");
    mSoundBuffers.load(Sounds::OpenDoor, "assets/sounds/open_door.wav");
}

void SoundPlayer::play(Sounds::ID sound)
{
    play(sound, getListenerPosition());
}

void SoundPlayer::play(Sounds::ID effect, sf::Vector2f position)
{
    mSounds.push_back(sf::Sound());
    sf::Sound& sound = mSounds.back();

    sound.setBuffer(mSoundBuffers.get(effect));
    sound.setPosition(position.x, position.y, 0.f);
    sound.setAttenuation(Attenuation);
    sound.setMinDistance(MinDistance3D);
    sound.setVolume(150.f);
    //sound.setPitch(randomFloat(-0.7f, 1.3f));

    sound.play();
}


void SoundPlayer::removeStoppedSounds()
{
    mSounds.remove_if([] (const sf::Sound& s) {
        return s.getStatus() == sf::Sound::Stopped;
    });
}

void SoundPlayer::setListenerPosition(sf::Vector2f position)
{
    sf::Listener::setPosition(position.x, -position.y, ListenerZ);
}

sf::Vector2f SoundPlayer::getListenerPosition() const
{
    return sf::Vector2f(sf::Listener::getPosition().x, sf::Listener::getPosition().y);
}
