#include "../XPSHelper/XPSHelper.hpp"

enum KLineDirection {
	DOWN_VERTICAL,
	DOWN_GORIZONTAL,

	UP_VERTICAL,
	UP_GORIZONTAL
};

enum KLineFrom {
	FROM_LEFT,
	FROM_RIGHT
};

class KLines {
public:
	KLines(const XPSHelper& xpsHelper);

	const std::vector<std::vector<int>>& GetField() const;
	std::vector<std::vector<int>> CreateBlockPath(int k, KLineDirection dir, KLineFrom) const;

private:
	const XPSHelper& xpsHelper_;
	std::vector<std::vector<int>> field_;
};