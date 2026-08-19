#include "CarAgenciesManager.h"

CarAgenciesManager::CarAgenciesManager():agency_manager(){
}

StatusType CarAgenciesManager::AddAgency(){
    try{
        agency_manager.AddCarAgency();
    }catch(std::bad_alloc& e){
        return ALLOCATION_ERROR;
    }
    return SUCCESS;
}

StatusType CarAgenciesManager::SellCar(int agencyID, int typeID, int k){
    if(k <= 0 || agencyID < 0){
        return INVALID_INPUT;
    }
    try{
        agency_manager.SellCar(agencyID, k, typeID);
    }catch(AgencyDoesntExists& e){
        return FAILURE;
    }catch(std::bad_alloc& e){
        return ALLOCATION_ERROR;
    }
    return SUCCESS;
}
StatusType CarAgenciesManager::UniteAgencies(int agencyID1, int agencyID2){
    if(agencyID1 < 0 || agencyID2 < 0){
        return INVALID_INPUT;
    }
    try{
        agency_manager.UniteCarAgencies(agencyID1, agencyID2);
    }catch(AgencyDoesntExists& e){
        return FAILURE;
    }catch(std::bad_alloc& e){
        return ALLOCATION_ERROR;
    }
    return SUCCESS;
}
StatusType CarAgenciesManager::GetIthSoldType(int agencyID, int i, int* res){
    if(i < 0 || agencyID < 0 || res == NULL){
        return INVALID_INPUT;
    }
    try {
        *res = agency_manager.GetIthSoldType(agencyID, i);
    }catch(InvalidIndex& e){
        return FAILURE;
    } catch(AgencyDoesntExists& e){
        return FAILURE;
    }catch(std::bad_alloc& e) {
        return ALLOCATION_ERROR;
    }

    return SUCCESS;
}

