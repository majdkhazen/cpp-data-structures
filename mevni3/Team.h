#ifndef WET1_TEAM_H
#define WET1_TEAM_H

#include "memory"
#include <iostream>
#include "Player.h"
#include "AVLTree.h"

class Player;

using std::shared_ptr;

class Team {
public:
    int team_id;
    int players_number;
    int points;
    int total_games, total_cards, total_goals;
    int goalKeepers;
    Player *top_scorer;

    comparePlayerid compare;
    comparePlayergoals compareplus;

    AVLTree<shared_ptr<Player>, comparePlayerid> *playersById;
    AVLTree<shared_ptr<Player>, comparePlayergoals> *playersByGoals;

    Team(int team_id, int points) : team_id(team_id), points(points), players_number(0),
                                    total_goals(0), total_cards(0), total_games(0),
                                    goalKeepers(0), top_scorer(nullptr),
                                    playersById(new AVLTree<shared_ptr<Player>, comparePlayerid>(compare)),
                                    playersByGoals(new AVLTree<shared_ptr<Player>, comparePlayergoals>(compareplus)) {}

    ~Team() {
        delete playersById;
        delete playersByGoals;
    }

    void updatePoints(int points);

    void update_team(int players_number, int total_cards, int total_goals, int goalKeepers);


};

class compareTeamid {
public:
    int operator()(const shared_ptr<Team> &t1, const shared_ptr<Team> &t2);
};


#endif //WET1_TEAM_H
