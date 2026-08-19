#ifndef MAIN_CPP_CAR_AGENCIES_MANAGER_H
#define MAIN_CPP_CAR_AGENCIES_MANAGER_H

#include "library.h"
#include "UnionFindAgency.h"

class CarAgenciesManager{
    UnionFindAgency agency_manager;
public:
    CarAgenciesManager();
    ~CarAgenciesManager() = default;
    StatusType AddAgency();
    StatusType SellCar(int agencyID, int typeID, int k);
    StatusType UniteAgencies(int agencyID1, int agencyID2);
    StatusType GetIthSoldType(int agencyID, int i, int* res);
};

#endif
