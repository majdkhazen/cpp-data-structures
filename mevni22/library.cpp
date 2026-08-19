#include "library.h"
#include "CarAgenciesManager.h"

void* Init(){
    CarAgenciesManager *DS = new CarAgenciesManager();
    return static_cast<void*>(DS);
}

StatusType AddAgency(void *DS) {
    if(DS == NULL) return INVALID_INPUT;
    return static_cast<CarAgenciesManager*>(DS)-> AddAgency();
}

StatusType SellCar(void *DS, int agencyID, int typeID, int k){
    if(DS == NULL) return INVALID_INPUT;
    return static_cast<CarAgenciesManager*>(DS)-> SellCar(agencyID, typeID, k);
}

StatusType UniteAgencies(void *DS, int agencyID1, int agencyID2){
    if(DS == NULL) return INVALID_INPUT;
    return static_cast<CarAgenciesManager*>(DS)-> UniteAgencies(agencyID1, agencyID2);
}

StatusType GetIthSoldType(void *DS, int agencyID, int i, int* res){
    if(DS == NULL) return INVALID_INPUT;
    return static_cast<CarAgenciesManager*>(DS)-> GetIthSoldType(agencyID, i, res);
}

void Quit(void** DS){
    CarAgenciesManager *to_destroy = static_cast<CarAgenciesManager*>(*DS);
    delete to_destroy;
    *DS = NULL;
}