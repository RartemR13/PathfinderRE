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
	std::pair<int, int> GetOldCoords(int num) const;

	bool IsNeighbours(int lhs, int rhs) const {
		const auto& [x1, y1] = GetWellCoords(lhs);
		const auto& [x2, y2] = GetWellCoords(rhs);

		std::vector<std::pair<int, int>> dirs = {{1, 0},
												 {0, 1},
												 {-1, 0},
												 {0, -1}};


		for (const auto& dir : dirs) {
			auto to = std::make_pair(x1 + dir.first, y1 + dir.second);
			if (to == std::make_pair(x2, y2))
				return true;
		}

		return false;
	}

private:
	std::vector<RArea> storage_;

	std::vector<std::pair<int, int>> geometry_;
	std::vector<std::pair<int, int>> oldGeometry_;

	std::map<std::pair<int, int>, int> geomToNum_;
};