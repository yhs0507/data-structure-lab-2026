#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include <cstdio>

#define MAX_DEGREE 80

class Polynomial {
    int degree;
    float coef[MAX_DEGREE];

public:
    Polynomial() {
        degree = 0;
        for (int i = 0; i < MAX_DEGREE; i++)
            coef[i] = 0;
    }

    void read() {
        printf("다항식의 최고 차수를 입력하시오: ");
        scanf("%d", &degree);

        printf("계수 입력 (%d개): ", degree + 1);
        for (int i = 0; i <= degree; i++) {
            scanf("%f", &coef[i]);
        }
    }

    void display(char* msg = (char*)" Poly = ") {
        printf("%s", msg);
        for (int i = 0; i < degree; i++) {
            printf("%5.1f x^%d + ", coef[i], degree - i);
        }
        printf("%4.1f\n", coef[degree]);
    }

    void add(Polynomial a, Polynomial b) {
        if (a.degree > b.degree) {
            *this = a;
            for (int i = 0; i <= b.degree; i++)
                coef[i + (degree - b.degree)] += b.coef[i];
        }
        else {
            *this = b;
            for (int i = 0; i <= a.degree; i++)
                coef[i + (degree - a.degree)] += a.coef[i];
        }
    }
};
