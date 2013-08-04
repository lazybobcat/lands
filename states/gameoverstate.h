#ifndef GAMEOVERSTATE_H
#define GAMEOVERSTATE_H

#include <SFML/Graphics/Text.hpp>

#include "state.h"

class GameOverState : public State
{
public:
    GameOverState(StateStack& stack, Context context);
    virtual void        draw();
    virtual bool        update(sf::Time dt);
    virtual bool        handleEvent(const sf::Event &event);

private:
    sf::Text            mText;

};

#endif // GAMEOVERSTATE_H
