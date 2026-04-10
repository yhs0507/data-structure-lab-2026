#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include <cstdio>
#include <cstring>

class Car {
protected:
    int speed;
    char name[40];

public:
    int gear;

    Car() {
        speed = 0;
        gear = 0;
        strcpy(name, "NoName");
    }

    Car(int s, char* n, int g) {
        speed = s;
        gear = g;
        strcpy(name, n);
    }

    void changeGear(int g = 4) {
        gear = g;
    }

    void speedUp() {
        speed += 5;
    }

    void display() {
        printf("[%s] : 기어=%d단 속도=%dkmph\n", name, gear, speed);
    }
};

class SportsCar : public Car {
public:
    bool bTurbo;

    void setTurbo(bool bTur) {
        bTurbo = bTur;
    }

    void speedUp() {
        if (bTurbo)
            speed += 20;
        else
            Car::speedUp();
    }
};
