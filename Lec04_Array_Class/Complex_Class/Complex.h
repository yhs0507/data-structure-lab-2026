#pragma once
#include <cstdio>

class Complex {
	double real;
	double imag;
public:
	void set(double r, double i) {
		real = r;
		imag = i;
	}
	void read(const char* msg = "복소수 = ") {
		printf(" %s ", msg);
		scanf_s("%lf \t %lf", &real, &imag);
	}
	void print(const char* msg = " 복소수 = ") {
		printf(" %s %4.2f + %4.2fi\n", msg, real, imag);
	}
	void add(Complex x, Complex y) {
		real = x.real + y.real;
		imag = x.imag + y.imag;
	}
};