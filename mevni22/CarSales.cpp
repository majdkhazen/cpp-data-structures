#include "CarSales.h"

CarSales::CarSales(int typeId, int sales): CarType(typeId, sales) {}

CarSales::CarSales(const CarSales& car_sales):CarType(car_sales) {}

bool CarSales::operator<(const CarSales& car_sales) const {
    if (sales != car_sales.sales) {
        return sales < car_sales.sales;
    }
    return type_id < car_sales.getTypeId();
}

bool operator>(const CarSales& car_sales1, const CarSales& car_sales2){
    return car_sales2 < car_sales1;
}
