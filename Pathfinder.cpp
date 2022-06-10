#include <iostream>

#include "XPSHelper\XPSHelper.hpp"
#include "TimeChecker\TimeChecker.hpp"
#include "G3\G3.hpp"

int main(int argc, char* argv[]) {
	TimeChecker checkTime;
	XPSHelper xps(argv[1]);
	G3 g3(xps, 10);


	return 0;
}