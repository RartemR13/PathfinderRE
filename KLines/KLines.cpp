#include "KLines.hpp"

KLines::KLines(const XPSHelper& xpsHelper) :
	xpsHelper_(xpsHelper)
{
	int maxX = 0,
		maxY = 0;

	for (int i = 0; i < xpsHelper_.GetWellCount(); ++i) {
		const auto& [x, y] = xpsHelper_.GetWellCoords(i);

		maxX = std::max(maxX, x),
		maxY = std::max(maxY, y); 
	}

	field_.resize(maxY+1, std::vector<int>(maxX+1, -1));
	for (int i = 0; i < xpsHelper_.GetWellCount(); ++i) {
		const auto& [x, y] = xpsHelper_.GetWellCoords(i);

		field_[y][x] = i;		
	}
}

const std::vector<std::vector<int>>& KLines::GetField() const {
	return field_;
}

std::vector<std::vector<int>> KLines::CreateBlockPath(int k, KLineDirection dir) const {
	std::vector<std::vector<int>> cur;

	switch(dir) {
		case KLineDirection::DOWN_VERTICAL:
		{
			int y = 0,
				addY = 1;
			int x = 0;

			while (x < field_.front().size()) {
				if (y < 0 or y >= field_.size()) {
					y -= addY;
					addY = -addY;
					x += k;
					continue;
				}

				std::vector<int> toAdd;
				for (int i = x; i < x + k and i < field_[y].size(); ++i) {
					if (field_[y][i] != -1)
						toAdd.push_back(field_[y][i]);
				}

				if (toAdd.size() > 0)
					cur.push_back(toAdd);

				y += addY;
			}

			break;
		}
	}

	bool fromFront = true;
	int wellCount = xpsHelper_.GetWellCount();

	int div = k,
		mod = wellCount % k;

	std::vector<std::vector<int>> ret;
	std::vector<int> flush;

	for (const auto& line : cur) {
		int curFlush = div + (mod > 0);
		mod--;

		if (fromFront) {
			for (int i = 0; i < line.size(); ++i)
				flush.push_back(line[i]);
		} else {
			for (int i = line.size()-1; i >= 0; --i)
				flush.push_back(line[i]);
		}

		fromFront = not fromFront;

		if (flush.size() >= curFlush) {
			std::vector<int> toAdd;
			toAdd.insert(toAdd.begin(), flush.begin(), flush.begin() + curFlush);
			flush.erase(flush.begin(), flush.begin() + curFlush);

			ret.push_back(toAdd);
		} else {
			mod++;
		}
	}

	if (flush.size() > 0)
		ret.push_back(flush);

	return ret;
}