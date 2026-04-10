#include "Car.h"

int main() {
    Car c1(50, (char*)"NormalCar", 3);
    SportsCar c2;

    c2.changeGear(2);
    c2.setTurbo(true);

    c1.speedUp();
    c2.speedUp();

    c1.display();
    c2.display();

    return 0;
}