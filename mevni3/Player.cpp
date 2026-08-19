#include "Player.h"

#include <iostream>

void Player::update_Player(int gamesPlayed, int scoredGoals, int cardsReceived)
{
    this->goals+=scoredGoals;
    this->cards+=cardsReceived;
    this->gamesPlayed+=gamesPlayed;
}

int comparePlayergoals::operator()(const shared_ptr<Player>& p1, const shared_ptr<Player>& p2)
{
    if(p1->goals < p2->goals)
    {
        return 1;
    }
    if(p1->goals==p2->goals)
    {
        if(p1->cards < p2->cards)
        {
            return -1;
        }
        if(p1->cards == p2->cards)
        {
            if(p1->playerId < p2->playerId)
            {
                return 1;
            }
            return -1;
        }
        return 1;
    }
    return -1;
}

int comparePlayerid::operator()(const shared_ptr<Player> &p1, const shared_ptr<Player> &p2)
{
    if(p1->playerId==p2->playerId)
    {
        return 0;
    }
    if(p1->playerId<p2->playerId)
    {
        return 1;
    }
    return -1;
}