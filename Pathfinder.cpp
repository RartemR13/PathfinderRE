#include <iostream>

#include "XPSHelper\XPSHelper.hpp"
#include "TimeChecker\TimeChecker.hpp"

int main(int argc, char* argv[]) {
	TimeChecker checkTime;
	auto checkTime2 = checkTime;
	XPSHelper xps(argv[1]);


	return 0;
}