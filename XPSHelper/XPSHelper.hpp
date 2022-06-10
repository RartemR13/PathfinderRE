#pragma once

#include "..\RArea\RArea.hpp"

#include <vector>
#include <map>

class XPSHelper {
public:
	XPSHelper(const char* XPSFilePath);

	int GetWellCount() const;
	int GetWellNum(std::pair<int, int> coords) const;
	std::pair<int, int> GetWellCoords(int num) const;
	RArea GetWellCoat(int num) const;

private:
	std::vector<RArea> storage_;

	std::vector<std::pair<int, int>> geometry_;
	std::vector<std::pair<int, int>> oldGeometry_;

	std::map<std::pair<int, int>, int> geomToNum_;
};