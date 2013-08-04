#ifndef PAUSESTATE_H
#define PAUSESTATE_H

#include <SFML/Graphics/Text.hpp>

#include "state.h"

class PauseState : public State
{
public:
    PauseState(StateStack& stack, Context context);

    virtual void        draw();
    virtual bool        update(sf::Time dt);
    virtual bool        handleEvent(const sf::Event &event);

private:
    sf::Text            mText;
};

#endif // PAUSESTATE_H
