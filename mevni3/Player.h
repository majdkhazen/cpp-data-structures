#ifndef WET1_PLAYER_H
#define WET1_PLAYER_H


#include "memory"
#include <iostream>

class Team;

using std::shared_ptr;



class Player{
public:
    int playerId;
    int teamId;
    int gamesPlayed;
    int goals;
    int cards;
    bool goalKeeper;
    Team* my_team;
    Player* Next;
    Player* prev;

    Player(int playerId,int teamId,int gamesPlayed,int goals,int cards,bool goalKeeper):playerId(playerId),
                                                                                        teamId(teamId),gamesPlayed(gamesPlayed),
                                                                                        goals(goals),cards(cards),goalKeeper(goalKeeper),
                                                                                        Next(nullptr),prev(nullptr){}
    ~Player()=default;
    void update_Player(int gamesPlayed,int scoredGoals,int cardsReceived);
};

class comparePlayergoals{
public:
    int operator()(const shared_ptr<Player>& p1, const shared_ptr<Player>& p2);
};

class comparePlayerid{
public:
    int operator()(const shared_ptr<Player>& p1, const shared_ptr<Player>& p2);
};

#endif //WET1_PLAYER_H
