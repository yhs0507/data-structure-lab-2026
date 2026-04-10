#include "Polynomial.h"

int main() {
    Polynomial a, b, c;

    a.read();
    b.read();

    c.add(a, b);

    a.display((char*)"A = ");
    b.display((char*)"B = ");
    c.display((char*)"A+B = ");

    return 0;
}