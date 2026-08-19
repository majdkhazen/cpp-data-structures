#include "UnionFindAgency.h"
#include "CarType.h"
#include "CarSales.h"
#include <iostream>
#define N 2

UnionFindAgency::UnionFindAgency():car_agencies(new CarAgency*[N * sizeof(CarAgency*)]) , max_size(N), curr_size(0) {
    for(int i = 0; i < N; i++){
        car_agencies[i] = NULL;
    }
}

UnionFindAgency::~UnionFindAgency(){
    for(int i = 0; i < curr_size; i++){
        //car_agencies[i]->sales_tree.empty();
        //car_agencies[i]->type_tree.empty();
        delete car_agencies[i];
    }
    delete [] car_agencies;
}

UnionFindAgency::CarAgency::CarAgency(int agency_id): agency_id(agency_id), tree_size(1), father(NULL), sales_tree(), type_tree(){}

void UnionFindAgency::AddCarAgency() {
    if (curr_size == max_size) {
        // No more place in array so make it bigger
        max_size *= 2;
        CarAgency **new_car_agencies = new CarAgency *[max_size * sizeof(CarAgency*)];
        for(int i = 0; i < max_size; i++) {
            if (i < curr_size) {
                new_car_agencies[i] = car_agencies[i];
            } else {
                new_car_agencies[i] = NULL;
            }
        }
        delete[] car_agencies;
        car_agencies = new_car_agencies;
    }

    CarAgency *new_car_agency = new CarAgency(curr_size);
    car_agencies[curr_size] = new_car_agency;
    curr_size++;
}

UnionFindAgency::CarAgency* UnionFindAgency::FindCarAgency(int agency){
    if(agency > max_size ||car_agencies[agency] == NULL) {
        throw AgencyDoesntExists();
    }
    return findAux(car_agencies[agency]);
}

UnionFindAgency::CarAgency* UnionFindAgency::findAux(CarAgency* agency) {
    if(agency->father == NULL) {
        return agency;
    }
    CarAgency* root = findAux(agency->father);
    agency->father = root;
    return root;
}

void UnionFindAgency::SellCar(int agency_id, int sales, int type_id) {
    CarAgency* car_agency = FindCarAgency(agency_id);
    CarType *car_type = NULL;
    if(!car_agency->type_tree.exist(CarType(type_id, 0))){
        car_agency->type_tree.insert(CarType(type_id, 0)); //new car in agency
        car_agency->sales_tree.insert(CarSales(type_id,0));
    }
    car_type = car_agency->type_tree.find(CarType(type_id, sales));

    //updating car sales in agency
    int updated_sells = sales + car_type->getSales();
    car_agency->sales_tree.remove(CarSales(type_id, car_type->getSales()));
    car_type->setSales(updated_sells);
    car_agency->sales_tree.insert(CarSales(type_id, updated_sells));
}

void UnionFindAgency::UniteCarAgencies(int agency_1, int agency_2){
    CarAgency* car_agency1 = FindCarAgency(agency_1);
    CarAgency* car_agency2 = FindCarAgency(agency_2);

    if(car_agency1->agency_id == car_agency2->agency_id)
        return;

    // Get merged trees
    AVLTree<CarSales> merged_sales(car_agency1->sales_tree, car_agency2->sales_tree);
    AVLTree<CarType> merged_types(car_agency1->type_tree, car_agency2->type_tree);
    int new_tree_size = car_agency1->tree_size + car_agency2->tree_size;

    // Empty old trees
    car_agency1->sales_tree.empty();
    car_agency1->type_tree.empty();
    car_agency2->sales_tree.empty();
    car_agency2->type_tree.empty();

    // Unite by agency size
    if(car_agency1->tree_size > car_agency2->tree_size){
        car_agency2->father = car_agency1;
        car_agency1->sales_tree = merged_sales;
        car_agency1->type_tree = merged_types;
        car_agency1->tree_size = new_tree_size;
    } else {
        car_agency1->father = car_agency2;
        car_agency2->sales_tree = merged_sales;
        car_agency2->type_tree = merged_types;
        car_agency2->tree_size = new_tree_size;
    }
}

//void UnionFindAgency::UniteCarAgencies(int agency_1, int agency_2){
//    CarAgency* car_agency1 = FindCarAgency(agency_1);
//    CarAgency* car_agency2 = FindCarAgency(agency_2);
//
//    // Merging both trees to array
//    AVLTree<CarType>::AvlIterator agency_it1 = car_agency1->sales_tree.begin();
//    AVLTree<CarType>::AvlIterator agency_it2 = car_agency2->sales_tree.begin();
//    int i = 0, new_tree_size = car_agency1->tree_size + car_agency2->tree_size;
//    CarType* arr_tree_unite = new CarType[new_tree_size * sizeof(CarType)];
//    while(agency_it1!= car_agency1->sales_tree.end() && agency_it2 != car_agency2->sales_tree.end()){
//        if(*agency_it1 < *agency_it2){
//            arr_tree_unite[i] = *(*agency_it1);
//            ++agency_it1;
//        }
//        else{
//            arr_tree_unite[i] = *(*agency_it2);
//            ++agency_it2;
//        }
//        i++;
//    }
//    while(agency_it1!= car_agency1->sales_tree.end()){
//        arr_tree_unite[i] = *(*agency_it1);
//        ++agency_it1;
//        i++;
//    }
//    while(agency_it2!= car_agency2->sales_tree.end()){
//        arr_tree_unite[i] = *(*agency_it2);
//        ++agency_it2;
//        i++;
//    }
//
//    car_agency1->sales_tree.empty();
//    car_agency2->sales_tree.empty();
//
//    //uniting agencies by agency size
//    if(car_agency1->tree_size > car_agency2->tree_size){
//        car_agency2->father = car_agency1;
//        car_agency1->sales_tree.sortedArrayInit(arr_tree_unite, new_tree_size);
//        car_agency1->tree_size = new_tree_size;
//    }else{
//        car_agency1->father = car_agency2;
//        car_agency2->sales_tree.sortedArrayInit(arr_tree_unite, new_tree_size);
//        car_agency1->tree_size = new_tree_size;
//    }
//    delete [] arr_tree_unite;
//}

int UnionFindAgency::GetIthSoldType(int agency_id, int i){
    CarAgency* car_agency = FindCarAgency(agency_id);
    CarSales* car_to_select = car_agency->sales_tree.select(i + 1);
    return car_to_select->getTypeId();
}
