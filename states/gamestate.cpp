#include "gamestate.h"

GameState::GameState(StateStack &stack, Context context) :
    State(stack, context),
    mWorld(*context.window, *context.textures, *context.fonts, *context.sounds, *context.scripts, mPlayer)
{
    context.music->play(Musics::MainTheme);
    context.music->setVolume(20.f);
}


void GameState::draw()
{
    sf::RenderWindow& window = *getContext().window;
    mWorld.draw();
}


bool GameState::update(sf::Time dt)
{
    mWorld.update(dt);

    // Mission status check
    if(!mWorld.hasPlayerAlive())
    {
        mPlayer.mMissionStatus = PlayerController::Failure;
        requestStackPush(States::GameOver);
    }
    else if(mPlayer.mMissionStatus == PlayerController::Success)
    {
        requestStackPush(States::GameVictory);
    }

    CommandQueue& commands = mWorld.getCommandQueue();
    mPlayer.handleRealtimeInput(commands);

    // If time, updte some scripted events

    return true;
}


bool GameState::handleEvent(const sf::Event &event)
{
    CommandQueue& commands = mWorld.getCommandQueue();
    mPlayer.handleEvent(event, commands);

    // If ESC key is pressed, we leave
    if (event.type == sf::Event::KeyReleased)
    {
        switch(event.key.code)
        {
            case sf::Keyboard::Escape:
                requestStackPush(States::Pause);
                break;

            default:break;
        }
    }

    return true;
}
