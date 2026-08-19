#include "CarType.h"

CarType::CarType(int type_id, int sales):type_id(type_id),sales(sales){

}

CarType::CarType(const CarType& car_type):type_id(car_type.type_id), sales(car_type.sales){

}

CarType& CarType::operator=(const CarType& car_type){
    if(this == &car_type){
        return *this;
    }
    type_id = car_type.type_id;
    sales = car_type.sales;
    return *this;
}

bool CarType::operator<(const CarType& car_type) const {
    return type_id < car_type.type_id;
}

bool CarType::operator==(const CarType& car_best_sells) const{
    return type_id == car_best_sells.type_id;
}


bool operator>(const CarType& car_type1, const CarType& car_type2){
    return car_type2 < car_type1;
}


int CarType::getTypeId() const{
    return type_id;
}

int CarType::getSales() const{
    return sales;
}

void CarType::setSales(int new_sales){
    sales = new_sales;
}
