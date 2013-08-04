#include "ghostcontroller.h"
#include "../entities/ghostentity.h"
#include "../entities/playerentity.h"
#include "../utils.h"

#include <iostream>

GhostController::GhostController()
{
}


void GhostController::update(sf::Time dt, CommandQueue &commands, GhostEntity &ghost)
{
    if(!mPlayer)
    {
        Command command;
        command.category = Category::Player;
        command.action = derivedAction<PlayerEntity>([this] (PlayerEntity& player, sf::Time) {
            mPlayer = &player;
        });

        commands.push(command);

        return;
    }

    if(ghost.mAction == Hunt && ghost.getHealthpoints() <= 25)
    {
        ghost.mAction = Escape;
        ghost.mRestTimer = sf::Time::Zero;
    }
    else if(ghost.mAction == Escape && ghost.mRestTimer >= sf::seconds(6.f))
    {
        ghost.heal(25);
        ghost.mRestTimer = sf::Time::Zero;
        ghost.mAction = Hunt;
        ghost.mDieAnimation.restart();
        Command command;
        command.category = Category::SoundEffect;
        command.action = derivedAction<SoundNode>(std::bind(&SoundNode::playSound, std::placeholders::_1, Sounds::GhostHunting, ghost.getWorldPosition()));

        commands.push(command);
    }
    else if(ghost.mAction == Escape)
    {
        ghost.mRestTimer += dt;
    }

    float dist = distance(*mPlayer, ghost);
    if(dist > 300.f)
        return;


    sf::Vector2f direction = unitVector(mPlayer->getWorldPosition() - ghost.getWorldPosition());

    switch(ghost.mAction)
    {
        case Escape:
            if(std::abs(direction.x) > std::abs(direction.y) && direction.x != 0.f)
            {
                if(direction.x > 0)
                {
                    ghost.move(Entity::Left);
                }
                else
                {
                    ghost.move(Entity::Right);
                }
            }
            else if(std::abs(direction.x) < std::abs(direction.y) && direction.y != 0.f)
            {
                if(direction.y > 0)
                {
                    ghost.move(Entity::Up);
                }
                else
                {
                    ghost.move(Entity::Down);
                }
            }
            break;

        case Hunt:
            if(std::abs(direction.x) > std::abs(direction.y) && direction.x != 0.f)
            {
                if(direction.x > 0)
                {
                    ghost.move(Entity::Right);
                }
                else
                {
                    ghost.move(Entity::Left);
                }
            }
            else if(std::abs(direction.x) < std::abs(direction.y) && direction.y != 0.f)
            {
                if(direction.y > 0)
                {
                    ghost.move(Entity::Down);
                }
                else
                {
                    ghost.move(Entity::Up);
                }
            }
            break;
    }
}
