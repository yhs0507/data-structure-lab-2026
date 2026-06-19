#include "Polynomial.h"

#include <iostream>
#include <windows.h>

int main() {
	SetConsoleOutputCP(65001);
	SetConsoleCP(65001);
	Polynomial a, b, c;
	a.read();
	b.read();
	c.add(a, b);
	a.display("A= ");
	b.display("B= ");
	c.display("C= ");

	return 0;
}