#pragma once
#include <iostream>
#include <cstring> 
#include <cstdio>   

class Car {
protected:
	int speed;
	char name[40];
public:
	int gear;
	Car() : speed(0), gear(1) {
		strcpy_s(name, "MyCar");
	}
	virtual ~Car() {}
	Car(int s, const char* n, int g)
		: speed(s), gear(g) {
		strcpy_s(name, n);
	}
	void changeGear(int g = 4) {
		gear = g;
	}
	void speedUp() {
		speed += 5;
	}
	void display() {
		printf("[%s] : Gear=%d Speed=%dkmph\n", name, gear, speed);
	}
	void whereAmI() {
		printf("Address=%p\n", (void*)this);
	}
};