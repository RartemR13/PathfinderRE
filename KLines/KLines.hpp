#include "../XPSHelper/XPSHelper.hpp"

enum KLineDirection {
	DOWN_VERTICAL,
	DOWN_GORIZONTAL,

	UP_VERTICAL,
	UP_GORIZONTAL
};

class KLines {
public:
	KLines(const XPSHelper& xpsHelper);

	const std::vector<std::vector<int>>& GetField() const;
	std::vector<std::vector<int>> CreateBlockPath(int k, KLineDirection dir) const;

private:
	const XPSHelper& xpsHelper_;
	std::vector<std::vector<int>> field_;
};