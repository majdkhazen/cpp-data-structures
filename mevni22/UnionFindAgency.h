#ifndef DS_HW2_UNION_FIND_AGENCY_H
#define DS_HW2_UNION_FIND_AGENCY_H

#include "CarSales.h"
#include "CarType.h"
#include "AVLRankTree.h"

class UnionFindAgency{

    class CarAgency {
        int agency_id;
        int tree_size;
        CarAgency *father;
        AVLTree<CarSales> sales_tree;
        AVLTree<CarType> type_tree;

    public:
        explicit CarAgency(int agency_id);
        ~CarAgency() = default;
        friend class UnionFindAgency;
    };
    CarAgency** car_agencies;
    int max_size;
    int curr_size;
    UnionFindAgency::CarAgency* findAux(CarAgency* agency);

public:
    UnionFindAgency();
    ~UnionFindAgency();

    void SellCar(int agency_id, int sales, int type_id);
    void AddCarAgency();
    UnionFindAgency::CarAgency* FindCarAgency(int agency);
    void UniteCarAgencies(int agency_1, int agency_2);
    int GetIthSoldType(int agencyID, int i);
};


#endif //DS_HW2_UNION_FIND_H
