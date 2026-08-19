#include <iostream>
#include "Team.h"


int compareTeamid::operator()(const shared_ptr<Team> &t1, const shared_ptr<Team> &t2)
{
    if(t1->team_id==t2->team_id)
    {
        return 0;
    }
    if(t1->team_id<t2->team_id)
    {
        return 1;
    }
    return -1;
}


void Team::updatePoints(int points)
{
        this->points+=points;
}

void Team::update_team(int players_number, int total_cards, int total_goals, int goalKeepers)
{
    this->total_goals+=total_goals;
    this->players_number+=players_number;
    this->total_cards+=total_cards;
    this->goalKeepers+=goalKeepers;
}