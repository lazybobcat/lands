#include "gameoverstate.h"
#include "../utils.h"

GameOverState::GameOverState(StateStack &stack, Context context) :
    State(stack, context),
    mText()
{
    mText.setFont(context.fonts->get(Fonts::Main));
    mText.setString("\t\tGame Over !\nPress ESCAPE to quit ...");
    centerOrigin(mText);
    mText.setPosition(context.window->getView().getSize() / 2.f);
}


void GameOverState::draw()
{
    sf::RenderWindow& window = *getContext().window;
    window.draw(mText);
}

bool GameOverState::update(sf::Time /*dt*/)
{
    return true;
}

bool GameOverState::handleEvent(const sf::Event &event)
{
    if (event.type == sf::Event::KeyReleased && event.key.code == sf::Keyboard::Escape)
    {
        requestStackClear();
    }

    return false;
}
