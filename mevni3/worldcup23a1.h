#ifndef WORLDCUP23A1_H_
#define WORLDCUP23A1_H_

#include "wet1util.h"
#include "AVLTree.h"
#include "Team.h"
#include "Player.h"

class world_cup_t {
public:
    int num_of_all_players;
    compareTeamid compareId;
    comparePlayerid comparePlayersId;
    comparePlayergoals compareGoals;

    AVLTree<shared_ptr<Team>,compareTeamid> AllTeams;
    AVLTree<shared_ptr<Player>,comparePlayerid> AllPlayers;
    AVLTree<shared_ptr<Player>,comparePlayergoals> AllPlayersByGoals;
    AVLTree<shared_ptr<Team>,compareTeamid> TeamsWith11Players;
    Player* topScorer;




    // <DO-NOT-MODIFY> {

    world_cup_t():topScorer(nullptr), AllTeams(compareId),
                  AllPlayers(comparePlayersId), AllPlayersByGoals(compareGoals),TeamsWith11Players(compareId),num_of_all_players(0){}
    virtual ~world_cup_t()=default;

    StatusType add_team(int teamId, int points);

    StatusType remove_team(int teamId);

    StatusType add_player(int playerId, int teamId, int gamesPlayed,
                          int goals, int cards, bool goalKeeper);

    StatusType remove_player(int playerId);

    StatusType update_player_stats(int playerId, int gamesPlayed,
                                   int scoredGoals, int cardsReceived);

    StatusType play_match(int teamId1, int teamId2);

    output_t<int> get_num_played_games(int playerId);

    output_t<int> get_team_points(int teamId);

    StatusType unite_teams(int teamId1, int teamId2, int newTeamId);

    output_t<int> get_top_scorer(int teamId);

    output_t<int> get_all_players_count(int teamId);

    StatusType get_all_players(int teamId, int *const output);

    output_t<int> get_closest_player(int playerId, int teamId);

    output_t<int> knockout_winner(int minTeamId, int maxTeamId);

    // } </DO-NOT-MODIFY>
};

#endif // WORLDCUP23A1_H_
