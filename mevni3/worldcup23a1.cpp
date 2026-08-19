#include "worldcup23a1.h"





static bool isTeamExist(world_cup_t& worldCup,int team_id){
    shared_ptr<Team> temp_team_ptr(new Team(team_id,0));
    return worldCup.AllTeams.isExist(temp_team_ptr);
}

static bool isTeamExist_aux(world_cup_t& worldCup,int team_id){
    shared_ptr<Team> temp_team_ptr(new Team(team_id,0));
    return worldCup.TeamsWith11Players.isExist(temp_team_ptr);
}

static shared_ptr<Team>  searchTeam(world_cup_t& worldCup,int team_id){
    shared_ptr<Team> temp_team_ptr=std::make_shared<Team>(team_id,0);
    //shared_ptr<Team> temp_team_ptr(new Team(team_id,0));
    if(isTeamExist(worldCup,team_id)) {
        temp_team_ptr = worldCup.AllTeams.binarySearch(worldCup.AllTeams.head, temp_team_ptr)->value;
        return temp_team_ptr;
    }
    return nullptr;
}

static Node<shared_ptr<Team>>*  searchTeam_aux(world_cup_t& worldCup,int team_id){
    shared_ptr<Team> temp_team_ptr(new Team(team_id,0));
    if(isTeamExist_aux(worldCup,team_id)) {
        return worldCup.TeamsWith11Players.binarySearch(worldCup.TeamsWith11Players.head, temp_team_ptr);
    }
    return nullptr;
}

static bool isPlayerExist(world_cup_t& worldCup,int player_id){
    shared_ptr<Player> temp_player_ptr(new Player(player_id,0,0,0,0, false));
    bool tmp=worldCup.AllPlayers.isExist(temp_player_ptr);
    return tmp;
}

static shared_ptr<Player> searchPlayer(world_cup_t& worldCup,int playerId){
    shared_ptr<Player> new_player(new Player(playerId,0,0,0,0, false));
    if(isPlayerExist(worldCup,playerId)){

        new_player = worldCup.AllPlayers.binarySearch(worldCup.AllPlayers.head,new_player)->value;
        return new_player;
    }
    return nullptr;
}

StatusType world_cup_t::add_team(int teamId, int points)
{
    // TODO: Your code goes here
    if(teamId <= 0 || points < 0){
        return StatusType::INVALID_INPUT;
    }
    shared_ptr<Team> new_team_ptr(new Team(teamId,points));
    if(new_team_ptr == nullptr){
        return StatusType::ALLOCATION_ERROR;
    }
    if(this->AllTeams.isExist(new_team_ptr))
        return StatusType::FAILURE;
    this->AllTeams.add(new_team_ptr);
    return StatusType::SUCCESS;
}

StatusType world_cup_t::remove_team(int teamId)
{
    if(teamId<=0) return StatusType::INVALID_INPUT;
    shared_ptr<Team> team=searchTeam(*this,teamId);
    if(!team) return StatusType::FAILURE;
    if(team->players_number!=0) return StatusType::FAILURE;
    if (this->AllTeams.isExist(team)){              //because in the AVLTree search according to compare class
        this->AllTeams.remove(team);           //which searches according to the id
        return StatusType::SUCCESS;
    }
    return StatusType::FAILURE;
}

StatusType world_cup_t::add_player(int playerId, int teamId, int gamesPlayed,
                                   int goals, int cards, bool goalKeeper)
{
    // TODO: Your code goes here
    if(playerId<=0 || teamId<=0 || gamesPlayed<0 || goals<0 || cards<0)
        return StatusType::INVALID_INPUT;
    if(gamesPlayed==0 && (goals>0 || cards>0))
        return StatusType::INVALID_INPUT;

    if(!isTeamExist(*this,teamId))  return StatusType::FAILURE;
    if(isPlayerExist(*this,playerId)) return StatusType::FAILURE;

    try {
        shared_ptr<Player> new_player(new Player(playerId,teamId,gamesPlayed,goals,cards,goalKeeper));
        this->num_of_all_players++;
        this->AllPlayers.add(new_player);
        this->AllPlayersByGoals.add(new_player);

        shared_ptr<Player> prev_player=this->AllPlayersByGoals.findPrev(new_player);

        if(prev_player)
        {
            if(prev_player->Next!=nullptr)
            {
                prev_player->Next->prev=new_player.get();
                new_player->Next=prev_player->Next;
                new_player->prev=prev_player.get();
                prev_player->Next= new_player.get();
            }
            else
            {
                prev_player->Next=new_player.get();
                new_player->prev=prev_player.get();
                new_player->Next= nullptr;
                topScorer = new_player.get();
            }
        }else{ // this is the smallest value it has no previous
            shared_ptr<Player> lowest = this->AllPlayersByGoals.getLeastValue()->value;
            new_player->Next = lowest.get();
        }
        if(topScorer == NULL){
            topScorer = new_player.get();
        } else if(new_player->goals > topScorer->goals){
            topScorer = new_player.get();
        } else if(new_player->prev == topScorer){
            topScorer = new_player.get();
        }


        //  * new player added to AllPlayers tree and AllPlayersByGoals, now we add the new player to his specific team *

        shared_ptr<Team> team = searchTeam(*this,teamId);
        if(new_player->goalKeeper)
            team->update_team(1,cards,goals,1);
        else
            team->update_team(1,cards,goals,0);
        new_player->gamesPlayed -= team->total_games;
        new_player->my_team = team.get();
        team->playersById->add(new_player);
        team->playersByGoals->add(new_player);
        if(team->players_number>=11 && team->goalKeepers>=1)
        {
            TeamsWith11Players.add(team);
        }

        if(team->top_scorer == NULL){
            team->top_scorer = new_player.get();
        } else if(new_player->goals > team->top_scorer->goals){
            team->top_scorer = new_player.get();
        } else if(new_player->goals == team->top_scorer->goals){
            if(new_player->cards < team->top_scorer->cards) {
                team->top_scorer = new_player.get();
            }else if(new_player->cards == team->top_scorer->cards){
                if(new_player->playerId > team->top_scorer->playerId){
                    team->top_scorer = new_player.get();
                }
            }
        }

        return StatusType::SUCCESS;
    }catch (...)
    {
        return StatusType::ALLOCATION_ERROR;
    }

}

StatusType world_cup_t::remove_player(int playerId)
{
    if(playerId<=0)
        return StatusType::INVALID_INPUT;
    shared_ptr<Player> wanted = searchPlayer(*this,playerId);
    if(wanted == nullptr)
        return StatusType::FAILURE;
    if((topScorer!=nullptr)&&(this->topScorer->playerId==playerId))
    {
        this->topScorer = this->topScorer->prev;
    }
    if(wanted->prev)
    {
        if(wanted->Next)
        {
            wanted->prev->Next=wanted->Next;
            wanted->Next->prev=wanted->prev;
        }
        else wanted->prev->Next=wanted->Next;
    }
    else
    {
        if(wanted->Next) wanted->Next->prev=wanted->prev;
    }
    AllPlayers.remove(wanted);
    AllPlayersByGoals.remove(wanted);
    num_of_all_players--;
    wanted->my_team->playersById->remove(wanted);
    wanted->my_team->playersByGoals->remove(wanted);
    if(wanted->goalKeeper) wanted->my_team->update_team(-1,-wanted->cards,-wanted->goals,-1);
    else wanted->my_team->update_team(-1,-wanted->cards,-wanted->goals,0);
//    shared_ptr<Team> tmp= std::make_shared<Team>(*wanted->my_team);
//    shared_ptr<Team> tmp=shared_ptr<Team>(wanted->my_team);
    if(wanted->my_team->players_number<11 || wanted->my_team->goalKeepers<1)
    {
        shared_ptr<Team> temp_team_ptr=std::make_shared<Team>(wanted->my_team->team_id,0);
        TeamsWith11Players.remove((temp_team_ptr));
    }
    // tmp = nullptr;
    return StatusType::SUCCESS;
}

StatusType world_cup_t::update_player_stats(int playerId, int gamesPlayed,
                                            int scoredGoals, int cardsReceived)
{

    if(playerId<=0 || gamesPlayed < 0 || scoredGoals <0 || cardsReceived<0)
        return StatusType::INVALID_INPUT;
    shared_ptr<Player> player = searchPlayer(*this,playerId);
    if(player == nullptr)
        return StatusType::FAILURE;

    this->remove_player(playerId);
    //shared_ptr<Player> new_player(new Player(playerId,player->teamId,player->gamesPlayed,player->goals,player->cards,player->goalKeeper));
//    AllPlayers.remove(player);
//    AllPlayersByGoals.remove(player);
//    player->my_team->playersById->remove(player);
//    player->my_team->playersByGoals->remove(player);
    player->update_Player(gamesPlayed,scoredGoals,cardsReceived);
    this->add_player(player->playerId,player->teamId,player->gamesPlayed,player->goals,player->cards,player->goalKeeper);
    return StatusType::SUCCESS;
}

StatusType world_cup_t::play_match(int teamId1, int teamId2)
{
    // TODO: Your code goes here
    if(teamId1<=0 || teamId2<=0 || teamId1 == teamId2)
        return StatusType::INVALID_INPUT;

    shared_ptr<Team> team1= searchTeam(*this,teamId1);
    shared_ptr<Team> team2= searchTeam(*this,teamId2);

    if(team1== nullptr || team2== nullptr)
        return StatusType::FAILURE;
    if(team1->players_number < 11 || team2->players_number < 11)
        return StatusType::FAILURE;
    if(!(team1->goalKeepers && team2->goalKeepers))
        return StatusType::FAILURE;

    int team1_score = team1->points + team1->total_goals - team1->total_cards;
    int team2_score = team2->points + team2->total_goals - team2->total_cards;


    if(team1_score == team2_score){
        team1->points+=1;
        team2->points+=1;
    } else if (team1_score > team2_score){
        team1->points+=3;
    } else{
        team2->points+=3;
    }
    team1->total_games+=1;
    team2->total_games+=1;
    return StatusType::SUCCESS;
}

output_t<int> world_cup_t::get_num_played_games(int playerId)
{
    // TODO: Your code goes here
    if(playerId <= 0){
        output_t<int> out(StatusType::INVALID_INPUT);
        return out;
    }
    shared_ptr<Player> player = searchPlayer(*this,playerId);
    if(player== nullptr){
        output_t<int> out(StatusType::FAILURE);
        return out;
    }
    return player->gamesPlayed + player->my_team->total_games;
}

output_t<int> world_cup_t::get_team_points(int teamId)
{
    // TODO: Your code goes here
    if(teamId<=0){
        output_t<int> out(StatusType::INVALID_INPUT);
        return out;
    }
    shared_ptr<Team> team =  searchTeam(*this,teamId);

    if(team== nullptr){
        output_t<int> out(StatusType::FAILURE);
        return out;
    }
    return team->points;
}

StatusType world_cup_t::unite_teams(int teamId1, int teamId2, int newTeamId)
{
    int x=0,y=0;
    if(teamId1<=0||teamId2<=0||newTeamId<=0||teamId1==teamId2) return StatusType::INVALID_INPUT;
    if(!isTeamExist(*this,teamId1)|| !isTeamExist(*this,teamId2)) return StatusType::FAILURE;
    shared_ptr<Team> tmp= searchTeam(*this,newTeamId);
    if((tmp!= nullptr) &&(tmp->team_id!=teamId1)&&(tmp->team_id!=teamId2))return StatusType::FAILURE;
    shared_ptr<Team> team1= searchTeam(*this,teamId1);
    shared_ptr<Team> team2= searchTeam(*this,teamId2);
    try
    {
        shared_ptr<Team> newteam(new Team(newTeamId,team1->points+team2->points));
        newteam->update_team(team1->players_number+team2->players_number,team1->total_cards+team2->total_cards
                ,team1->total_goals+team2->total_goals,team1->goalKeepers+team2->goalKeepers);

        shared_ptr<Player>* arr1= new shared_ptr<Player>[team1->players_number];
        shared_ptr<Player>* arr2= new shared_ptr<Player>[team2->players_number];

        team1->playersById->inOrder(team1->playersById->head,team1->players_number,x,arr1);
        team2->playersById->inOrder(team2->playersById->head,team2->players_number,y,arr2);

        for(int i=0;i<team1->players_number;i++)
        {
            arr1[i]->gamesPlayed+=team1->total_games;
            arr1[i]->my_team=newteam.get();
        }
        for(int j=0;j<team2->players_number;j++)
        {
            arr2[j]->gamesPlayed+=team1->total_games;
            arr2[j]->my_team=newteam.get();
        }
        AVLTree<shared_ptr<Player>,comparePlayerid>* merged=team1->playersById->mergedAVL(team2->playersById,team1->playersById);
        AVLTree<shared_ptr<Player>,comparePlayergoals>* merged1=team1->playersByGoals->mergedAVL(team2->playersByGoals,team1->playersByGoals);
        newteam->playersById=merged;
        newteam->playersByGoals=merged1;

        AllTeams.remove(team1);
        AllTeams.remove(team2);
        AllTeams.add(newteam);

        if(isTeamExist_aux(*this,team1->team_id)) TeamsWith11Players.remove(team1);
        if(isTeamExist_aux(*this,team2->team_id)) TeamsWith11Players.remove(team2);
        if(newteam->players_number>=1 && newteam->goalKeepers>=1)
        {
            TeamsWith11Players.add(newteam);
        }
        shared_ptr<Player> top=newteam->playersByGoals->rightson(newteam->playersByGoals->head);
        topScorer=top.get();
        delete [] arr1;
        delete [] arr2;
        return StatusType::SUCCESS;
    }
    catch (...){
        return StatusType::ALLOCATION_ERROR;
    }
}

output_t<int> world_cup_t::get_top_scorer(int teamId)
{
    // TODO: Your code goes here
    if(teamId == 0){
        output_t<int> out(StatusType::INVALID_INPUT);
        return out;
    }
    if(teamId<0){
        if(this->topScorer!=NULL)
            return this->topScorer->playerId;
        output_t<int> out(StatusType::FAILURE);
        return out;
    }

    shared_ptr<Team> team= searchTeam(*this, teamId);

    if(team.get()==NULL){
        output_t<int> out(StatusType::FAILURE);
        return out;
    }

    if(team->players_number <=0){
        output_t<int> out(StatusType::FAILURE);
        return out;
    }

    if(team == nullptr){
        output_t<int> out(StatusType::FAILURE);
        return out;
    }
    return team->top_scorer->playerId;
}

output_t<int> world_cup_t::get_all_players_count(int teamId)
{
    if(teamId==0){
        output_t<int> out(StatusType::INVALID_INPUT);
        return out;
    }
    if(teamId<0){
        return this->num_of_all_players;
    }

    shared_ptr<Team> team= searchTeam(*this, teamId);
    if(team == nullptr){
        output_t<int> out(StatusType::FAILURE);
        return out;
    }
    return team->players_number;
}

StatusType world_cup_t::get_all_players(int teamId, int *const output)
{
    if(teamId == 0 || output==NULL){
        return StatusType::INVALID_INPUT;
    }
    int index=0;
    if(teamId<0){
        if (this->AllPlayersByGoals.size==0){
            return StatusType::FAILURE;
        }
        shared_ptr<Player> *players_arr = new shared_ptr<Player>[this->num_of_all_players];
        this->AllPlayersByGoals.inOrder(this->AllPlayersByGoals.head,this->num_of_all_players,index,players_arr);
        /// shared_ptr<Player> == index+1 at least this what supposed to be///
        for(int i=0; i<index; i++){
            output[i]=players_arr[i]->playerId;
        }
        return StatusType::SUCCESS;
    }
    if(!isTeamExist(*this,teamId)){
        return StatusType::FAILURE;
    }
    shared_ptr<Team> team = searchTeam(*this,teamId);
    if(team->players_number <=0){
        return StatusType::FAILURE;
    }

    auto *players_arr = new shared_ptr<Player>[team->players_number];
    team->playersByGoals->inOrder(team->playersByGoals->head,team->players_number,index,players_arr);
    for(int i=0; i<index; i++){
        output[i]=players_arr[i]->playerId;
    }
    return StatusType::SUCCESS;
}


int get_closestAux(Player* prev_player, Player* next_player, Player* my_player) {
    int diff_next = next_player->goals - my_player->goals;
    int diff_prev = my_player->goals - prev_player->goals;
    if (diff_next == diff_prev) {
        diff_next = my_player->cards - next_player->cards;
        diff_prev = prev_player->cards - my_player->cards;
        diff_prev = diff_prev <0? -1*diff_prev : diff_prev;
        diff_next = diff_next <0? -1*diff_next : diff_next;
        if (diff_next == diff_prev) {
            diff_next = next_player->playerId - my_player->playerId;
            diff_prev = my_player->playerId - prev_player->playerId;
        }
    }
    diff_prev = diff_prev <0? -1*diff_prev : diff_prev;
    diff_next = diff_next <0? -1*diff_next : diff_next;
    return diff_next > diff_prev ? prev_player->playerId : next_player->playerId;
}

output_t<int> world_cup_t::get_closest_player(int playerId, int teamId)
{
    output_t<int> fail_out(StatusType::FAILURE);

    if(playerId <=0 || teamId <= 0){
        output_t<int> out(StatusType::INVALID_INPUT);
        return  out;
    }
    if(!isTeamExist(*this, teamId) || this->num_of_all_players<=1){
        return fail_out;
    }

    shared_ptr<Team> team = searchTeam(*this, teamId);
    shared_ptr<Player> temp_player_ptr(new Player(playerId,0,0,0,0, false));
    if(!team->playersById->isExist(temp_player_ptr)){
        return fail_out;
    }
    shared_ptr<Player> my_player = team->playersById->binarySearch(team->playersById->head,temp_player_ptr)->value;
    if(my_player->Next==nullptr)
        return my_player->prev->playerId;
    if(my_player->prev==nullptr)
        return my_player->Next->playerId;

    // shared_ptr<Player> next_player(new Player(my_player->Next->playerId,0,0,0,0, 0));


    output_t<int> closest(get_closestAux(my_player->prev,my_player->Next,my_player.get()));
    return closest;
}



output_t<int> world_cup_t::knockout_winner(int minTeamId, int maxTeamId)
{
    int count=0;
    bool add_min=false,add_max=false;
    if(minTeamId<0||maxTeamId<0||minTeamId>maxTeamId) return StatusType::INVALID_INPUT;
    try
    {
        shared_ptr<Team> min(new Team(minTeamId,0));
        shared_ptr<Team> max(new Team(maxTeamId,0));
        Node<shared_ptr<Team>>* new_min;
        Node<shared_ptr<Team>>* new_max;
        if(!isTeamExist_aux(*this,minTeamId))
        {
            TeamsWith11Players.add(min);
            add_min=true;
            count++;
            shared_ptr<Team> next=TeamsWith11Players.findNext(min);
            if(next== nullptr)
            {
                TeamsWith11Players.remove(min);
                return StatusType::FAILURE;
            }
            new_min= searchTeam_aux(*this,next->team_id);
        }
        else new_min= searchTeam_aux(*this,minTeamId);
        if(!isTeamExist_aux(*this,maxTeamId))
        {
            TeamsWith11Players.add(max);
            add_max=true;
            count++;
            shared_ptr<Team> prev=TeamsWith11Players.findPrev(max);
            if(prev== nullptr)
            {
                TeamsWith11Players.remove(max);
                return StatusType::FAILURE;
            }
            new_max= searchTeam_aux(*this,prev->team_id);
        }
        else new_max= searchTeam_aux(*this,maxTeamId);


        Node<shared_ptr<Team>>* ancestor=TeamsWith11Players.ancestor(TeamsWith11Players.head,new_min,new_max);

        shared_ptr<Team>* arr1=new shared_ptr<Team>[TeamsWith11Players.size];
        int index=0;
        TeamsWith11Players.inOrder_aux(ancestor,new_min,new_max,TeamsWith11Players.size,index,arr1);
        if(index==0)
        {
            TeamsWith11Players.remove(min);
            TeamsWith11Players.remove(max);
            return StatusType::FAILURE;
        }
        int* arr_id=new int[index];
        int* arr_points=new int[index];
        for(int i=0;i<index;i++)
        {
            arr_id[i]=arr1[i]->team_id;
            arr_points[i]=(arr1[i]->points+arr1[i]->total_goals-arr1[i]->total_cards);
        }
        if (index==1)
        {
            output_t<int> success(arr_id[0]);
            return success;
        }
        int ind=0;
        while (index!=1)
        {
            if(index%2==0)
            {
                for(int i=0;i<index-1;i+=2)
                {
                    if(arr_points[i]==arr_points[i+1])
                    {
                        if(arr_id[i]>arr_id[i+1])
                        {
                            arr_points[ind]=arr_points[i]+arr_points[i+1]+3;
                            arr_id[ind]=arr_id[i];
                            ind++;
                        }
                        else
                        {
                            arr_points[ind]=arr_points[i]+arr_points[i+1]+3;
                            arr_id[ind]=arr_id[i+1];
                            ind++;
                        }
                    }
                    else
                    {
                        if(arr_points[i] > arr_points[i+1])
                        {
                            arr_points[ind]=arr_points[i]+arr_points[i+1]+3;
                            arr_id[ind]=arr_id[i];
                            ind++;
                        }
                        else
                        {
                            arr_points[ind]=arr_points[i]+arr_points[i+1]+3;
                            arr_id[ind]=arr_id[i+1];
                            ind++;
                        }
                    }
                }
                index/=2;
                ind=0;
            }
            else
            {
                int k=0;
                for(int j=0;j<index-2;j+=2)
                {
                    if(arr_points[j]==arr_points[j+1])
                    {
                        if(arr_id[j]>arr_id[j+1])
                        {
                            arr_points[ind]=arr_points[j]+arr_points[j+1]+3;
                            arr_id[ind]=arr_id[j];
                            ind++;
                            k+=2;
                        }
                        else
                        {
                            arr_points[ind]=arr_points[j]+arr_points[j+1]+3;
                            arr_id[ind]=arr_id[j+1];
                            ind++;
                            k+=2;
                        }
                    }
                    else
                    {
                        if(arr_points[j] > arr_points[j+1])
                        {
                            arr_points[ind]=arr_points[j]+arr_points[j+1]+3;
                            arr_id[ind]=arr_id[j];
                            ind++;
                            k+=2;
                        }
                        else
                        {
                            arr_points[ind]=arr_points[j]+arr_points[j+1]+3;
                            arr_id[ind]=arr_id[j+1];
                            ind++;
                            k+=2;
                        }
                    }
                }
                index=index/2+1;
                arr_points[ind]=arr_points[k];
                arr_id[ind]=arr_id[k];
                ind=0;
            }
        }
        if(add_min) TeamsWith11Players.remove(min);
        if(add_max) TeamsWith11Players.remove(max);
        output_t<int> winner(arr_id[0]);
        return winner;
    }
    catch (...)
    {
        return StatusType::ALLOCATION_ERROR;
    }
}