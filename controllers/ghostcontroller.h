#ifndef GHOSTCONTROLLER_H
#define GHOSTCONTROLLER_H

#include "../events/commandqueue.h"


class GhostEntity;
class PlayerEntity;

class GhostController
{
public:
    enum Action
    {
        Hunt,
        Escape
    };

public:
    GhostController();

    void update(sf::Time dt, CommandQueue& commands, GhostEntity& ghost);


private:
    PlayerEntity*   mPlayer = nullptr;
};

#endif // GHOSTCONTROLLER_H
