#pragma once
#include "Car.h"

class SportsCar : public Car {
    bool bTurbo;

public:
    // 생성자: 부모 클래스의 생성자 Car(s, n, g)를 호출
    SportsCar(int s, const char* n, int g) : Car(s, n, g), bTurbo(false) {}

    void setTurbo(bool bTur) {
        bTurbo = bTur;
    }

    // 메서드 오버라이딩
    void speedUp() {
        if (bTurbo) speed += 20;
        else Car::speedUp();
    }
};
