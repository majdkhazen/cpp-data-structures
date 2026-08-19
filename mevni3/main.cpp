#include <iostream>
#include "worldcup23a1.h"
#define CATCH_CONFIG_MAIN

#include <string>
#include <sstream>
#include <vector>
#include <stdlib.h>
#include "worldcup23a1.h"
#include <string>
#define TEST_CASE(x) std::cout <<"\n\n" x << std::endl;

void PrinT( StatusType s){
    if(s == StatusType::FAILURE){
        std::cout << " FAILURE" << std::endl;
        return;
    }
    if( s == StatusType::INVALID_INPUT){
        std::cout << " INVALID_INPUT" << std::endl;
        return;
    }
    if(s == StatusType::SUCCESS){
        std::cout << " SUCCESS" << std::endl;
        return;
    }
}

void REQUIRE(bool b){
    if(b){
        std::cout << " SUCCESS" << std::endl;
    }
    else
        std::cout << " FLAIL -----------" << std::endl;
}

void SECTION(std::string s){
    std::cout << "\n" << std::endl ;
    std::cout << s << std::endl;
}

int main(){

    SECTION("unite_teams failure");
    {
        world_cup_t *obj = new world_cup_t();
        StatusType res = obj->unite_teams(1, 2, 1);
        REQUIRE(res == StatusType::FAILURE);

        res = obj->add_team(1, 2);
        REQUIRE(res == StatusType::SUCCESS);
        res = obj->unite_teams(1, 2, 5);
        REQUIRE(res == StatusType::FAILURE);

        res = obj->add_team(2, 2);
        REQUIRE(res == StatusType::SUCCESS);
        res = obj->add_team(5, 2);
        REQUIRE(res == StatusType::SUCCESS);
        res = obj->unite_teams(1, 2, 5);
        REQUIRE(res == StatusType::FAILURE);

        delete obj;
    }

    SECTION("unite_teams invalid input");
    {
        world_cup_t *obj = new world_cup_t();
        StatusType res = obj->unite_teams(0, 1, 1);
        REQUIRE(res == StatusType::INVALID_INPUT);
        res = obj->unite_teams(1, 0, 1);
        REQUIRE(res == StatusType::INVALID_INPUT);
        res = obj->unite_teams(1, 2, 0);
        REQUIRE(res == StatusType::INVALID_INPUT);
        res = obj->unite_teams(-1, 1, 1);
        REQUIRE(res == StatusType::INVALID_INPUT);
        res = obj->unite_teams(1, -1, 1);
        REQUIRE(res == StatusType::INVALID_INPUT);
        res = obj->unite_teams(1, 3, -1);
        REQUIRE(res == StatusType::INVALID_INPUT);
        res = obj->unite_teams(1, 1, 1);
        REQUIRE(res == StatusType::INVALID_INPUT);

        delete obj;
    }

    SECTION("unite_teams simple");
    {
        world_cup_t *obj = new world_cup_t();
        StatusType res = obj->add_team(1, 2);
        REQUIRE(res == StatusType::SUCCESS);
        res = obj->add_player(1001, 1, 1, 5, 2, true);
        REQUIRE(res == StatusType::SUCCESS);
        res = obj->add_team(2, 2);
        REQUIRE(res == StatusType::SUCCESS);
        res = obj->add_player(1002, 2, 1, 3, 4, false);
        REQUIRE(res == StatusType::SUCCESS);
        res = obj->unite_teams(1, 2, 3);
        REQUIRE(res == StatusType::SUCCESS);
        int players[10] = {};
        res = obj->get_all_players(3, players);
        REQUIRE(res == StatusType::SUCCESS);
        REQUIRE(players[0] == 1002);
        REQUIRE(players[1] == 1001);
        REQUIRE(players[2] == 0);
        REQUIRE(players[3] == 0);
        REQUIRE(players[4] == 0);
        REQUIRE(players[5] == 0);
        REQUIRE(players[6] == 0);
        REQUIRE(players[7] == 0);
        REQUIRE(players[8] == 0);
        REQUIRE(players[9] == 0);

        res = obj->add_team(3, 2);
        REQUIRE(res == StatusType::FAILURE);
        res = obj->add_team(1, 2);
        REQUIRE(res == StatusType::SUCCESS);
        res = obj->add_team(2, 2);
        REQUIRE(res == StatusType::SUCCESS);
        res = obj->add_team(4, 2);
        REQUIRE(res == StatusType::SUCCESS);
        res = obj->add_player(1001, 3, 1, 5, 2, true);
        REQUIRE(res == StatusType::FAILURE);
        res = obj->add_player(1002, 3, 1, 3, 4, false);
        REQUIRE(res == StatusType::FAILURE);
        res = obj->add_player(1003, 3, 1, 1, 1, false);
        REQUIRE(res == StatusType::SUCCESS);
        res = obj->add_player(1004, 4, 1, 1, 2, true);
        REQUIRE(res == StatusType::SUCCESS);
        res = obj->add_player(1005, 4, 1, 1, 4, false);
        REQUIRE(res == StatusType::SUCCESS);
        res = obj->add_player(1006, 4, 1, 1, 1, false);
        REQUIRE(res == StatusType::SUCCESS);
        res = obj->add_player(1007, 4, 1, 1, 1, false);
        REQUIRE(res == StatusType::SUCCESS);
        res = obj->unite_teams(3, 4, 5);
        REQUIRE(res == StatusType::SUCCESS);
        res = obj->add_team(5, 2);
        REQUIRE(res == StatusType::FAILURE);
        res = obj->add_team(3, 2);
        REQUIRE(res == StatusType::SUCCESS);
        res = obj->add_team(4, 2);
        REQUIRE(res == StatusType::SUCCESS);
        res = obj->add_player(1008, 3, 1, 1, 1, false);
        REQUIRE(res == StatusType::SUCCESS);
        res = obj->add_player(1004, 3, 1, 1, 1, false);
        REQUIRE(res == StatusType::FAILURE);
        res = obj->add_player(1009, 4, 1, 1, 1, false);
        REQUIRE(res == StatusType::SUCCESS);
        res = obj->add_player(1005, 4, 1, 1, 1, false);
        REQUIRE(res == StatusType::FAILURE);
        res = obj->unite_teams(3, 4, 3);
        REQUIRE(res == StatusType::SUCCESS);
        res = obj->add_team(3, 2);
        REQUIRE(res == StatusType::FAILURE);

        res = obj->add_team(4, 2);
        REQUIRE(res == StatusType::SUCCESS);
        res = obj->add_player(1009, 3, 1, 1, 1, false);
        REQUIRE(res == StatusType::FAILURE);

        delete obj;
    }

    SECTION("unite_teams that makes it legal team");
    {
        world_cup_t *obj = new world_cup_t();

        StatusType res = obj->add_team(1, 2);
        REQUIRE(res == StatusType::SUCCESS);
        res = obj->add_player(1001, 1, 1, 5, 2, true);
        REQUIRE(res == StatusType::SUCCESS);
        res = obj->add_player(1002, 1, 1, 3, 4, false);
        REQUIRE(res == StatusType::SUCCESS);
        res = obj->add_player(1003, 1, 1, 1, 1, false);
        REQUIRE(res == StatusType::SUCCESS);
        res = obj->add_player(1004, 1, 1, 1, 2, true);
        REQUIRE(res == StatusType::SUCCESS);
        res = obj->add_player(1005, 1, 1, 1, 4, false);
        REQUIRE(res == StatusType::SUCCESS);
        res = obj->add_team(2, 2);
        REQUIRE(res == StatusType::SUCCESS);
        res = obj->add_player(1006, 2, 1, 1, 1, false);
        REQUIRE(res == StatusType::SUCCESS);
        res = obj->add_player(1007, 2, 1, 1, 1, false);
        REQUIRE(res == StatusType::SUCCESS);
        res = obj->add_player(1008, 2, 1, 1, 1, false);
        REQUIRE(res == StatusType::SUCCESS);
        res = obj->add_player(1009, 2, 1, 1, 1, false);
        REQUIRE(res == StatusType::SUCCESS);
        res = obj->add_player(1010, 2, 1, 1, 1, false);
        REQUIRE(res == StatusType::SUCCESS);
        res = obj->add_player(1011, 2, 1, 1, 1, false);
        REQUIRE(res == StatusType::SUCCESS);
        res = obj->add_player(1012, 2, 1, 1, 1, false);
        REQUIRE(res == StatusType::SUCCESS);
        res = obj->add_player(1013, 2, 1, 1, 1, false);
        REQUIRE(res == StatusType::SUCCESS);
        res = obj->add_player(1014, 2, 1, 1, 1, false);
        REQUIRE(res == StatusType::SUCCESS);
        res = obj->add_player(1015, 2, 1, 1, 1, true);
        REQUIRE(res == StatusType::SUCCESS);
        res = obj->add_player(1016, 2, 1, 1, 1, false);
        REQUIRE(res == StatusType::SUCCESS);
        res = obj->add_player(1017, 2, 1, 1, 1, false);
        REQUIRE(res == StatusType::SUCCESS);
        output_t<int> resn3 = obj->knockout_winner(1, 4);
        REQUIRE(resn3.status() == StatusType::SUCCESS);
        REQUIRE(resn3.ans() == 2);

        res = obj->unite_teams(1, 2, 3);
        REQUIRE(res == StatusType::SUCCESS);

        output_t<int> resn1 = obj->get_all_players_count(3);
        REQUIRE(resn1.status() == StatusType::SUCCESS);
        REQUIRE(resn1.ans() == 17);
        output_t<int> resn2 = obj->knockout_winner(1, 4);
        REQUIRE(resn2.status() == StatusType::SUCCESS);
        REQUIRE(resn2.ans() == 3);

        delete obj;
    }

    SECTION("unite_teams two legal teams");
    {
        world_cup_t *obj = new world_cup_t();

        StatusType res = obj->add_team(1, 2);
        REQUIRE(res == StatusType::SUCCESS);
        res = obj->add_player(1001, 1, 1, 5, 2, true);
        REQUIRE(res == StatusType::SUCCESS);
        res = obj->add_player(1002, 1, 1, 3, 4, false);
        REQUIRE(res == StatusType::SUCCESS);
        res = obj->add_player(1003, 1, 1, 1, 1, false);
        REQUIRE(res == StatusType::SUCCESS);
        res = obj->add_player(1004, 1, 1, 1, 2, true);
        REQUIRE(res == StatusType::SUCCESS);
        res = obj->add_player(1005, 1, 1, 1, 4, false);
        REQUIRE(res == StatusType::SUCCESS);
        res = obj->add_player(1301, 1, 1, 5, 2, true);
        REQUIRE(res == StatusType::SUCCESS);
        res = obj->add_player(1302, 1, 1, 3, 4, false);
        REQUIRE(res == StatusType::SUCCESS);
        res = obj->add_player(1303, 1, 1, 1, 1, false);
        REQUIRE(res == StatusType::SUCCESS);
        res = obj->add_player(1304, 1, 1, 1, 2, true);
        REQUIRE(res == StatusType::SUCCESS);
        res = obj->add_player(1305, 1, 1, 1, 4, false);
        REQUIRE(res == StatusType::SUCCESS);
        res = obj->add_player(101, 1, 1, 5, 2, true);
        REQUIRE(res == StatusType::SUCCESS);
        res = obj->add_player(333, 1, 1, 3, 4, false);
        REQUIRE(res == StatusType::SUCCESS);
        res = obj->add_player(103, 1, 1, 1, 1, false);
        REQUIRE(res == StatusType::SUCCESS);
        res = obj->add_player(6004, 1, 1, 1, 2, true);
        REQUIRE(res == StatusType::SUCCESS);
        res = obj->add_player(19005, 1, 1, 1, 4, false);
        REQUIRE(res == StatusType::SUCCESS);
        res = obj->add_team(2, 2);
        REQUIRE(res == StatusType::SUCCESS);
        res = obj->add_player(1006, 2, 1, 1, 1, false);
        REQUIRE(res == StatusType::SUCCESS);
        res = obj->add_player(1007, 2, 1, 1, 1, false);
        REQUIRE(res == StatusType::SUCCESS);
        res = obj->add_player(1008, 2, 1, 1, 1, false);
        REQUIRE(res == StatusType::SUCCESS);
        res = obj->add_player(1009, 2, 1, 1, 1, false);
        REQUIRE(res == StatusType::SUCCESS);
        res = obj->add_player(1010, 2, 1, 1, 1, false);
        REQUIRE(res == StatusType::SUCCESS);
        res = obj->add_player(1011, 2, 1, 1, 1, false);
        REQUIRE(res == StatusType::SUCCESS);
        res = obj->add_player(1012, 2, 1, 1, 1, false);
        REQUIRE(res == StatusType::SUCCESS);
        res = obj->add_player(1013, 2, 1, 1, 1, false);
        REQUIRE(res == StatusType::SUCCESS);
        res = obj->add_player(1014, 2, 1, 1, 1, false);
        REQUIRE(res == StatusType::SUCCESS);
        res = obj->add_player(1015, 2, 1, 1, 1, true);
        REQUIRE(res == StatusType::SUCCESS);
        res = obj->add_player(1016, 2, 1, 1, 1, false);
        REQUIRE(res == StatusType::SUCCESS);
        res = obj->add_player(1017, 2, 1, 1, 1, false);
        REQUIRE(res == StatusType::SUCCESS);
        output_t<int> resn3 = obj->knockout_winner(1, 4);
        REQUIRE(resn3.status() == StatusType::SUCCESS);
        REQUIRE(resn3.ans() == 2);

        res = obj->unite_teams(1, 2, 3);
        REQUIRE(res == StatusType::SUCCESS);

        output_t<int> resn1 = obj->get_all_players_count(3);
        REQUIRE(resn1.status() == StatusType::SUCCESS);
        REQUIRE(resn1.ans() == 27);
        output_t<int> resn2 = obj->knockout_winner(1, 4);
        REQUIRE(resn2.status() == StatusType::SUCCESS);
        REQUIRE(resn2.ans() == 3);

        delete obj;
    }

    SECTION("unite_teams legal and not legal teams to the not legal");
    {
        world_cup_t *obj = new world_cup_t();

        StatusType res = obj->add_team(1, 2);
        REQUIRE(res == StatusType::SUCCESS);
        res = obj->add_player(1001, 1, 1, 5, 2, false);
        REQUIRE(res == StatusType::SUCCESS);
        res = obj->add_player(1002, 1, 1, 3, 4, false);
        REQUIRE(res == StatusType::SUCCESS);
        res = obj->add_player(1003, 1, 1, 1, 1, false);
        REQUIRE(res == StatusType::SUCCESS);
        res = obj->add_team(2, 2);
        REQUIRE(res == StatusType::SUCCESS);
        res = obj->add_player(1006, 2, 1, 1, 1, false);
        REQUIRE(res == StatusType::SUCCESS);
        res = obj->add_player(1007, 2, 1, 1, 1, false);
        REQUIRE(res == StatusType::SUCCESS);
        res = obj->add_player(1008, 2, 1, 1, 1, false);
        REQUIRE(res == StatusType::SUCCESS);
        res = obj->add_player(1009, 2, 1, 1, 1, false);
        REQUIRE(res == StatusType::SUCCESS);
        res = obj->add_player(1010, 2, 1, 1, 1, false);
        REQUIRE(res == StatusType::SUCCESS);
        res = obj->add_player(1011, 2, 1, 1, 1, false);
        REQUIRE(res == StatusType::SUCCESS);
        res = obj->add_player(1012, 2, 1, 1, 1, false);
        REQUIRE(res == StatusType::SUCCESS);
        res = obj->add_player(1013, 2, 1, 1, 1, false);
        REQUIRE(res == StatusType::SUCCESS);
        res = obj->add_player(1014, 2, 1, 1, 1, false);
        REQUIRE(res == StatusType::SUCCESS);
        res = obj->add_player(1015, 2, 1, 1, 1, true);
        REQUIRE(res == StatusType::SUCCESS);
        res = obj->add_player(1016, 2, 1, 1, 1, false);
        REQUIRE(res == StatusType::SUCCESS);
        res = obj->add_player(1017, 2, 1, 1, 1, false);
        REQUIRE(res == StatusType::SUCCESS);
        output_t<int> resn3 = obj->knockout_winner(1, 4);
        REQUIRE(resn3.status() == StatusType::SUCCESS);
        REQUIRE(resn3.ans() == 2);

        res = obj->unite_teams(1, 2, 1);
        REQUIRE(res == StatusType::SUCCESS);

        output_t<int> resn1 = obj->get_all_players_count(1);
        REQUIRE(resn1.status() == StatusType::SUCCESS);
        REQUIRE(resn1.ans() == 15);
        output_t<int> resn2 = obj->knockout_winner(1, 4);
        REQUIRE(resn2.status() == StatusType::SUCCESS);
        REQUIRE(resn2.ans() == 1);

        delete obj;
    }

    SECTION("ILLIGAL_INPUTS");
    {
        world_cup_t *obj = new world_cup_t();

        output_t<int> res1 = obj->knockout_winner(-1, 0);
        REQUIRE(res1.status() == StatusType::INVALID_INPUT);

        output_t<int> res2 = obj->knockout_winner(-4, -1);
        REQUIRE(res2.status() == StatusType::INVALID_INPUT);

        output_t<int> res3 = obj->knockout_winner(0, -1);
        REQUIRE(res3.status() == StatusType::INVALID_INPUT);

        output_t<int> res4 = obj->knockout_winner(-1, -1);
        REQUIRE(res4.status() == StatusType::INVALID_INPUT);

        output_t<int> res5 = obj->knockout_winner(10, 9);
        REQUIRE(res5.status() == StatusType::INVALID_INPUT);

        output_t<int> res5andahalf = obj->knockout_winner(0, 1);
        REQUIRE(res5andahalf.status() == StatusType::FAILURE);

        StatusType res = obj->add_team(7, 10);
        REQUIRE(res == StatusType::SUCCESS);
        res = obj->add_team(9, 10);
        REQUIRE(res == StatusType::SUCCESS);
        res = obj->add_player(69, 7, 9, 1, 1, false);
        REQUIRE(res == StatusType::SUCCESS);

        output_t<int> res6 = obj->knockout_winner(0, 10);
        REQUIRE(res6.status() == StatusType::FAILURE);

        output_t<int> res7 = obj->knockout_winner(7, 7);
        REQUIRE(res7.status() == StatusType::FAILURE);

        output_t<int> res8 = obj->knockout_winner(7, 9);
        REQUIRE(res8.status() == StatusType::FAILURE);

        output_t<int> res9 = obj->knockout_winner(7, 10);
        REQUIRE(res9.status() == StatusType::FAILURE);

        output_t<int> res10 = obj->knockout_winner(6, 9);
        REQUIRE(res10.status() == StatusType::FAILURE);

        output_t<int> res11 = obj->knockout_winner(0, 1);
        REQUIRE(res11.status() == StatusType::FAILURE);

        res = obj->add_player(70, 7, 9, 1, 1, false);
        REQUIRE(res == StatusType::SUCCESS);
        res = obj->add_player(71, 7, 9, 1, 1, false);
        REQUIRE(res == StatusType::SUCCESS);
        res = obj->add_player(72, 7, 9, 1, 1, false);
        REQUIRE(res == StatusType::SUCCESS);
        res = obj->add_player(73, 7, 9, 1, 1, false);
        REQUIRE(res == StatusType::SUCCESS);
        res = obj->add_player(74, 7, 9, 1, 1, false);
        REQUIRE(res == StatusType::SUCCESS);
        res = obj->add_player(75, 7, 9, 1, 1, false);
        REQUIRE(res == StatusType::SUCCESS);
        res = obj->add_player(76, 7, 9, 1, 1, false);
        REQUIRE(res == StatusType::SUCCESS);
        res = obj->add_player(77, 7, 9, 1, 1, false);
        REQUIRE(res == StatusType::SUCCESS);
        res = obj->add_player(78, 7, 9, 1, 1, false);
        REQUIRE(res == StatusType::SUCCESS);
        res = obj->add_player(79, 7, 9, 1, 1, false);
        REQUIRE(res == StatusType::SUCCESS);
        res = obj->add_player(80, 7, 9, 1, 1, true);
        REQUIRE(res == StatusType::SUCCESS);

        output_t<int> res12 = obj->knockout_winner(0, 6);
        REQUIRE(res12.status() == StatusType::FAILURE);

        output_t<int> res13 = obj->knockout_winner(8, 9);
        REQUIRE(res13.status() == StatusType::FAILURE);

        delete obj;
        std::cout << "enddddddd" << std::endl;
    }
    SECTION("Basic Tournements");
    {
        world_cup_t *obj2 = new world_cup_t();
        int playerId = 1;
        StatusType res;
        for (int teamId = 1; teamId < 10; teamId += 2)
        {
            res = obj2->add_team(teamId, 1000 / ((6 - teamId) * (6 - teamId)));
            REQUIRE(res == StatusType::SUCCESS);

            for (int startingId = playerId; playerId < startingId + 10 + teamId; ++playerId)
            {
                res = obj2->add_player(playerId, teamId, 1, playerId, 3, true);
                REQUIRE(res == StatusType::SUCCESS);
            }
            ++playerId;
        }
        // Strengths team1:73 team3:319 team5:1485 team7:1816 team9:1384

        res = obj2->add_team(4, 1000000);
        REQUIRE(res == StatusType::SUCCESS);

        output_t<int> res1 = obj2->knockout_winner(8, 9);
        REQUIRE(res1.status() == StatusType::SUCCESS);
        REQUIRE(res1.ans() == 9);

        output_t<int> res2 = obj2->knockout_winner(1, 1);
        REQUIRE(res2.status() == StatusType::SUCCESS);
        REQUIRE(res2.ans() == 1);

        output_t<int> res3 = obj2->knockout_winner(2, 4);
        REQUIRE(res3.status() == StatusType::SUCCESS);
        REQUIRE(res3.ans() == 3);

        output_t<int> res4 = obj2->knockout_winner(3, 9);
        REQUIRE(res4.status() == StatusType::SUCCESS);
        REQUIRE(res4.ans() == 7);

        output_t<int> res5 = obj2->knockout_winner(2, 8);
        REQUIRE(res5.status() == StatusType::SUCCESS);
        REQUIRE(res5.ans() == 7);

        output_t<int> res6 = obj2->knockout_winner(0, 5);
        REQUIRE(res6.status() == StatusType::SUCCESS);
        REQUIRE(res6.ans() == 5);

        output_t<int> res7 = obj2->knockout_winner(1, 9);
        REQUIRE(res7.status() == StatusType::SUCCESS);
        REQUIRE(res7.ans() == 7);

        res = obj2->add_player(999, 3, 1, 3000, 200, false);
        REQUIRE(res == StatusType::SUCCESS);
        // Strengths team1:73 team3:3119 team5:1485 team7:1816 team9:1384

        output_t<int> res8 = obj2->knockout_winner(1, 999);
        REQUIRE(res8.status() == StatusType::SUCCESS);
        REQUIRE(res8.ans() == 7);

        res = obj2->add_player(998, 3, 1, 2000, 0, false);
        REQUIRE(res == StatusType::SUCCESS);
        // Strengths team1:73 team3: 5119 team5:1485 team7:1816 team9:1384

        output_t<int> res9 = obj2->knockout_winner(0, 13);
        REQUIRE(res9.status() == StatusType::SUCCESS);
        REQUIRE(res9.ans() == 3);

        res = obj2->add_player(997, 3, 1, 1, 5001, false);
        REQUIRE(res == StatusType::SUCCESS);
        // Strengths team1:73 team3:119 team5:1485 team7:1816 team9:1384

        output_t<int> res9andahalf = obj2->knockout_winner(0, 13);
        REQUIRE(res9andahalf.status() == StatusType::SUCCESS);
        REQUIRE(res9andahalf.ans() == 7);

        res = obj2->update_player_stats(79, 1, 10000, 0);
        REQUIRE(res == StatusType::SUCCESS);
        // Strengths team1:73 team3:119 team5:1485 team7:1816 team9:11384

        output_t<int> res10 = obj2->knockout_winner(1, 9);
        REQUIRE(res10.status() == StatusType::SUCCESS);

        REQUIRE(res10.ans() == 9);
        std::cout << " ************************************* " << std::endl;

        delete obj2;
        std::cout << " ************************************* " << std::endl;

    }


//    SECTION("ILLIGAL_INPUTS");
//    {
//        world_cup_t *obj = new world_cup_t();
//
//        output_t<int> res1 = obj->knockout_winner(-1, 0);
//        REQUIRE(res1.status() == StatusType::INVALID_INPUT);
//
//        output_t<int> res2 = obj->knockout_winner(-4, -1);
//        REQUIRE(res2.status() == StatusType::INVALID_INPUT);
//
//        output_t<int> res3 = obj->knockout_winner(0, -1);
//        REQUIRE(res3.status() == StatusType::INVALID_INPUT);
//
//        output_t<int> res4 = obj->knockout_winner(-1, -1);
//        REQUIRE(res4.status() == StatusType::INVALID_INPUT);
//
//        output_t<int> res5 = obj->knockout_winner(10, 9);
//        REQUIRE(res5.status() == StatusType::INVALID_INPUT);
//
//        output_t<int> res5andahalf = obj->knockout_winner(0, 1);
//        REQUIRE(res5andahalf.status() == StatusType::FAILURE);
//
//        StatusType res = obj->add_team(7, 10);
//        REQUIRE(res == StatusType::SUCCESS);
//        res = obj->add_team(9, 10);
//        REQUIRE(res == StatusType::SUCCESS);
//        res = obj->add_player(69, 7, 9, 1, 1, false);
//        REQUIRE(res == StatusType::SUCCESS);
//
//        output_t<int> res6 = obj->knockout_winner(0, 10);
//        REQUIRE(res6.status() == StatusType::FAILURE);
//
//        output_t<int> res7 = obj->knockout_winner(7, 7);
//        REQUIRE(res7.status() == StatusType::FAILURE);
//
//        output_t<int> res8 = obj->knockout_winner(7, 9);
//        REQUIRE(res8.status() == StatusType::FAILURE);
//
//        output_t<int> res9 = obj->knockout_winner(7, 10);
//        REQUIRE(res9.status() == StatusType::FAILURE);
//
//        output_t<int> res10 = obj->knockout_winner(6, 9);
//        REQUIRE(res10.status() == StatusType::FAILURE);
//
//        output_t<int> res11 = obj->knockout_winner(0, 1);
//        REQUIRE(res11.status() == StatusType::FAILURE);
//
//        res = obj->add_player(70, 7, 9, 1, 1, false);
//        REQUIRE(res == StatusType::SUCCESS);
//        res = obj->add_player(71, 7, 9, 1, 1, false);
//        REQUIRE(res == StatusType::SUCCESS);
//        res = obj->add_player(72, 7, 9, 1, 1, false);
//        REQUIRE(res == StatusType::SUCCESS);
//        res = obj->add_player(73, 7, 9, 1, 1, false);
//        REQUIRE(res == StatusType::SUCCESS);
//        res = obj->add_player(74, 7, 9, 1, 1, false);
//        REQUIRE(res == StatusType::SUCCESS);
//        res = obj->add_player(75, 7, 9, 1, 1, false);
//        REQUIRE(res == StatusType::SUCCESS);
//        res = obj->add_player(76, 7, 9, 1, 1, false);
//        REQUIRE(res == StatusType::SUCCESS);
//        res = obj->add_player(77, 7, 9, 1, 1, false);
//        REQUIRE(res == StatusType::SUCCESS);
//        res = obj->add_player(78, 7, 9, 1, 1, false);
//        REQUIRE(res == StatusType::SUCCESS);
//        res = obj->add_player(79, 7, 9, 1, 1, false);
//        REQUIRE(res == StatusType::SUCCESS);
//        res = obj->add_player(80, 7, 9, 1, 1, true);
//        REQUIRE(res == StatusType::SUCCESS);
//
//        output_t<int> res12 = obj->knockout_winner(0, 6);
//        REQUIRE(res12.status() == StatusType::FAILURE);
//
//        output_t<int> res13 = obj->knockout_winner(8, 9);
//        REQUIRE(res13.status() == StatusType::FAILURE);
//
//        delete obj;
//    }
//
//    SECTION("Basic Tournements");
//    {
//        world_cup_t *obj = new world_cup_t();
//        int playerId = 1;
//        StatusType res;
//        for (int teamId = 1; teamId < 10; teamId += 2)
//        {
//            res = obj->add_team(teamId, 1000 / ((6 - teamId) * (6 - teamId)));
//            REQUIRE(res == StatusType::SUCCESS);
//
//            for (int startingId = playerId; playerId < startingId + 10 + teamId; ++playerId)
//            {
//                res = obj->add_player(playerId, teamId, 1, playerId, 3, true);
//                REQUIRE(res == StatusType::SUCCESS);
//            }
//            ++playerId;
//        }
//        // Strengths team1:73 team3:319 team5:1485 team7:1816 team9:1384
//
//        res = obj->add_team(4, 1000000);
//        REQUIRE(res == StatusType::SUCCESS);
//
//        output_t<int> res1 = obj->knockout_winner(8, 9);
//        REQUIRE(res1.status() == StatusType::SUCCESS);
//        REQUIRE(res1.ans() == 9);
//
//        output_t<int> res2 = obj->knockout_winner(1, 1);
//        REQUIRE(res2.status() == StatusType::SUCCESS);
//        REQUIRE(res2.ans() == 1);
//
//        output_t<int> res3 = obj->knockout_winner(2, 4);
//        REQUIRE(res3.status() == StatusType::SUCCESS);
//        REQUIRE(res3.ans() == 3);
//
//        output_t<int> res4 = obj->knockout_winner(3, 9);
//        REQUIRE(res4.status() == StatusType::SUCCESS);
//        REQUIRE(res4.ans() == 7);
//
//        output_t<int> res5 = obj->knockout_winner(2, 8);
//        REQUIRE(res5.status() == StatusType::SUCCESS);
//        REQUIRE(res5.ans() == 7);
//
//        output_t<int> res6 = obj->knockout_winner(0, 5);
//        REQUIRE(res6.status() == StatusType::SUCCESS);
//        REQUIRE(res6.ans() == 5);
//
//        output_t<int> res7 = obj->knockout_winner(1, 9);
//        REQUIRE(res7.status() == StatusType::SUCCESS);
//        REQUIRE(res7.ans() == 7);
//
//        res = obj->add_player(999, 3, 1, 3000, 200, false);
//        REQUIRE(res == StatusType::SUCCESS);
//        // Strengths team1:73 team3:3119 team5:1485 team7:1816 team9:1384
//
//        output_t<int> res8 = obj->knockout_winner(1, 999);
//        REQUIRE(res8.status() == StatusType::SUCCESS);
//        REQUIRE(res8.ans() == 7);
//
//        res = obj->add_player(998, 3, 1, 2000, 0, false);
//        REQUIRE(res == StatusType::SUCCESS);
//        // Strengths team1:73 team3: 5119 team5:1485 team7:1816 team9:1384
//
//        output_t<int> res9 = obj->knockout_winner(0, 13);
//        REQUIRE(res9.status() == StatusType::SUCCESS);
//        REQUIRE(res9.ans() == 3);
//
//        res = obj->add_player(997, 3, 1, 1, 5001, false);
//        REQUIRE(res == StatusType::SUCCESS);
//        // Strengths team1:73 team3:119 team5:1485 team7:1816 team9:1384
//
//        output_t<int> res9andahalf = obj->knockout_winner(0, 13);
//        REQUIRE(res9andahalf.status() == StatusType::SUCCESS);
//        REQUIRE(res9andahalf.ans() == 7);
//
//        res = obj->update_player_stats(79, 1, 10000, 0);
//        REQUIRE(res == StatusType::SUCCESS);
//        // Strengths team1:73 team3:119 team5:1485 team7:1816 team9:11384
//
//        output_t<int> res10 = obj->knockout_winner(1, 9);
//        REQUIRE(res10.status() == StatusType::SUCCESS);
//
//        REQUIRE(res10.ans() == 9);
//
//        delete obj;
//    }
    return 0;
}

