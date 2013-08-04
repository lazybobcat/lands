#ifndef GAMEVICTORYSTATE_H
#define GAMEVICTORYSTATE_H

#include "state.h"

#include <SFML/Graphics/Text.hpp>

class GameVictoryState : public State
{
public:
    GameVictoryState(StateStack& stack, Context& context);

    virtual void draw();
    virtual bool update(sf::Time dt);
    virtual bool handleEvent(const sf::Event &event);

private:
    sf::Text        mText;
};

#endif // GAMEVICTORYSTATE_H
