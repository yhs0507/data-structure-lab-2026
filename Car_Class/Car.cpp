#include "SportsCar.h"

int main() {
    SportsCar car;

    car.changeGear(3);
    car.setTurbo(true);
    car.speedUp();
    car.display();
    car.whereAmI();

    return 0;
}