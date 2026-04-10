#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <cstring>

class Car {
protected:
    int speed;
    char name[40];

public:
    int gear;

    // 기본 생성자
    Car() : speed(0), gear(1) {
        strcpy(name, "Car");
    }

    // 매개변수가 있는 생성자
    Car(int s, const char* n, int g) : speed(s), gear(g) {
        strcpy(name, n);
    }

    ~Car() {}

    void changeGear(int g = 4) {
        gear = g;
    }

    void speedUp() {
        speed += 5;
    }

    void display() {
        printf("[%s] : 기어=%d, 속도=%dkmph\n", name, gear, speed);
    }

    void whereAmI() {
        printf("주소 = %p\n", (void*)this);
    }
};