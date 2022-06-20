#include <iostream>

#include "XPSHelper\XPSHelper.hpp"
#include "TimeChecker\TimeChecker.hpp"
#include "G3\G3.hpp"
#include "PathHelper\PathHelper.hpp"

int main(int argc, char* argv[]) {
	TimeChecker checkTime;
	XPSHelper xps(argv[1]);
	G3 g3(xps, 20);

	std::ofstream out("vis.txt", std::ios::out);
	PathHelper ph(g3);

	out << g3.GetXPSHelper().GetWellCount() << "\n";
	for (int i = 0; i < g3.GetXPSHelper().GetWellCount(); ++i)
		out << g3.GetXPSHelper().GetWellCoords(i).first << " " << g3.GetXPSHelper().GetWellCoords(i).second << "\n";
	out << "\n";
	ph.FindPath(std::vector<int>{0, 40, 80, 120, 160, 200, 240, 280, 320, 360,
								 400, 440, 480, 520, 560, 600, 640, 680, 720, 760});
	ph.Optimize();

	ph.WritePath(out);
	out.close();

	return 0;
}