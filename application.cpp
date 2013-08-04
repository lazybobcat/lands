#include "application.h"
#include "utils.h"
#include "states/titlestate.h"
#include "states/gamestate.h"
#include "states/pausestate.h"
#include "states/gameoverstate.h"
#include "states/gamevictorystate.h"

const sf::Time Application::TimePerFrame = sf::seconds(1.f/60.f);

Application::Application(unsigned int width, unsigned int height, const std::string &title) :
    mWindow(sf::VideoMode(width,height), title.c_str(), sf::Style::Close),
    mTextures(),
    mFonts(),
    mPlayer(),
    mStateStack(State::Context(mWindow, mTextures, mFonts, mSounds, mMusic, mScripts, mPlayer)),
    mStatisticsText(),
    mStatisticsUpdateTime(),
    mStatisticsNumFrames(0)
{
    mWindow.setKeyRepeatEnabled(false);


    // Resources
    mFonts.load(Fonts::Main, "assets/fonts/sansation.ttf");
    mFonts.load(Fonts::Flipps, "assets/fonts/flipps.otf");

    mTextures.load(Textures::Particle, "assets/textures/particle.png");

    mStatisticsText.setFont(mFonts.get(Fonts::Main));
    mStatisticsText.setPosition(5.f,5.f);
    mStatisticsText.setCharacterSize(10);

    registerStates();
    exposeToLua();
    mStateStack.pushState(States::Title);
}



void Application::run()
{
    sf::Clock clock;
    sf::Time timeSinceLastUpdate = sf::Time::Zero;
    while(mWindow.isOpen())
    {
        sf::Time elapsedTime = clock.restart();
        timeSinceLastUpdate += elapsedTime;
        while(timeSinceLastUpdate > TimePerFrame)
        {
            timeSinceLastUpdate -= TimePerFrame;

            processEvents();
            update(TimePerFrame);

            // Check inside this loop, because stack might be empty before update() call
            if (mStateStack.isEmpty())
                mWindow.close();
        }

        updateStatistics(elapsedTime);
        render();
    }
}

void Application::processEvents()
{
    sf::Event event;
    while(mWindow.pollEvent(event))
    {
        mStateStack.handleEvent(event);
        if(event.type == sf::Event::Closed)
            mWindow.close();
    }
}

void Application::update(sf::Time dt)
{
    mStateStack.update(dt);
}

void Application::render()
{
    mWindow.clear();
    mStateStack.draw();

    mWindow.setView(mWindow.getDefaultView());
    mWindow.draw(mStatisticsText);
    mWindow.display();
}

void Application::registerStates()
{
    mStateStack.registerState<TitleState>(States::Title);
    mStateStack.registerState<GameState>(States::Game);
    mStateStack.registerState<GameOverState>(States::GameOver);
    mStateStack.registerState<PauseState>(States::Pause);
    mStateStack.registerState<GameVictoryState>(States::GameVictory);
}

void Application::updateStatistics(sf::Time elapsedTime)
{
    mStatisticsUpdateTime += elapsedTime;
    mStatisticsNumFrames += 1;

    if(mStatisticsUpdateTime >= sf::seconds(1.f))
    {
        mStatisticsText.setString(
                    "Frames/sec = " + toString(mStatisticsNumFrames) + "\n" +
                    "Time/update = " + toString(mStatisticsUpdateTime.asMicroseconds() / mStatisticsNumFrames) + "us"
        );

        mStatisticsNumFrames = 0;
        mStatisticsUpdateTime -= sf::seconds(1.f);
    }
}
