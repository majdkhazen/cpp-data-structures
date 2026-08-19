#ifndef DS_HW2_CARSALES_H
#define DS_HW2_CARSALES_H

#include "CarType.h"

class CarSales: public CarType{
public:
    CarSales(int typeId = -1, int sales = -1);
    ~CarSales()= default;
    bool operator<(const CarSales&) const;
    CarSales(const CarSales&);
};

bool operator>(const CarSales& car_sales1, const CarSales& car_sales2);


#endif //DS_HW2_CARSALES_H
