#pragma once

#include "..\RArea\RArea.hpp"

#include <vector>
#include <map>

class XPSHelper {
public:
	XPSHelper(const char* XPSFilePath);

	int GetWellCount();
	int GetWellNum(std::pair<int, int> coords);
	std::pair<int, int> GetWellCoords(int num);
	RArea GetWellCoat(int num);

private:
	std::vector<RArea> storage_;
	std::vector<std::pair<int, int>> geometry_;

	std::map<std::pair<int, int>, int> geomToNum_;
};