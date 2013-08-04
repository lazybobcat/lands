#ifndef GAMESTATE_H
#define GAMESTATE_H

#include <SFML/Graphics.hpp>

#include "state.h"
#include "../world.h"
#include "../controllers/playercontroller.h"

class GameState : public State
{
public:
    GameState(StateStack& stack, Context context);

    virtual void draw();
    virtual bool update(sf::Time dt);
    virtual bool handleEvent(const sf::Event &event);

private:
    World           mWorld;
    PlayerController mPlayer;
};

#endif // GAMESTATE_H
