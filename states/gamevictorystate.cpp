#include "gamevictorystate.h"
#include "../utils.h"
#include "../world.h"

GameVictoryState::GameVictoryState(StateStack& stack, Context& context) :
    State(stack, context),
    mText()
{
    mText.setFont(context.fonts->get(Fonts::Main));
    mText.setString("You're the best player ever ! Gratz !\n\n\tPress SPACE !");
    centerOrigin(mText);
    mText.setPosition(context.window->getView().getSize() / 2.f);
}

void GameVictoryState::draw()
{
    sf::RenderWindow& window = *getContext().window;
    window.draw(mText);
}

bool GameVictoryState::update(sf::Time /*dt*/)
{
    return false;
}

bool GameVictoryState::handleEvent(const sf::Event &event)
{
    if (event.type == sf::Event::KeyReleased)
    {
        if(event.key.code == sf::Keyboard::Escape || event.key.code == sf::Keyboard::Space)
        {
            requestStackClear();
            if(ActualLevel+1 < Maps::EndOfMapList)
            {
                ActualLevel = static_cast<Maps::ID>(ActualLevel+1);
                requestStackPush(States::Game);
            }
        }
    }

    return false;
}

