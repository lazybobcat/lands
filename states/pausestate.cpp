#include "pausestate.h"
#include "../utils.h"

PauseState::PauseState(StateStack &stack, Context context) :
    State(stack, context),
    mText()
{
    mText.setFont(context.fonts->get(Fonts::Main));
    mText.setString("\t\tGame Paused !\n\nPress SPACE to continue ...\nPress ESCAPE to quit ...");
    centerOrigin(mText);
    mText.setPosition(context.window->getView().getSize() / 2.f);
    context.music->pause(true);
}

void PauseState::draw()
{
    sf::RenderWindow& window = *getContext().window;

    sf::RectangleShape back;
    back.setFillColor(sf::Color(0,0,0, 150));
    back.setSize(window.getView().getSize());

    window.draw(back);
    window.draw(mText);
}


bool PauseState::update(sf::Time /*dt*/)
{
    return false;
}

bool PauseState::handleEvent(const sf::Event &event)
{
    if (event.type == sf::Event::KeyReleased && event.key.code == sf::Keyboard::Space)
    {
        requestStackPop();
        getContext().music->pause(false);
    }
    else if (event.type == sf::Event::KeyReleased && event.key.code == sf::Keyboard::Escape)
    {
        requestStackClear();
    }

    return false;
}
