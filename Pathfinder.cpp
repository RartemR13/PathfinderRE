#include <iostream>

#include "XPSHelper\XPSHelper.hpp"
#include "TimeChecker\TimeChecker.hpp"
#include "G3\G3.hpp"
#include "PathHelper\PathHelper.hpp"
#include "KLines/KLines.hpp"

void WriteXPSPath(const std::vector<std::vector<int>>& path,
				  const XPSHelper& xpsHelper,
				  std::ofstream& out)
{
	for (int i = 0; i < path.size(); ++i) {
		for (int j = 0; j < path[i].size(); ++j)
			out << xpsHelper.GetOldCoords(path[i][j]).first << " " <<
				   xpsHelper.GetOldCoords(path[i][j]).second << " " << 
				   i << "\n";
	}
}

void WritePath(const std::vector<std::vector<int>>& path,
			   const XPSHelper& xpsHelper,
			   std::ofstream& out)
{
	out << xpsHelper.GetWellCount() << "\n";
	for (int i = 0; i < xpsHelper.GetWellCount(); ++i)
		out << xpsHelper.GetWellCoords(i).first << " " << xpsHelper.GetWellCoords(i).second << "\n";
	out << "\n";

	out << path.size() << "\n\n";

	for(const auto& vertexes : path) {
		out << vertexes.size() << " 0" << "\n";
		for(const auto& vertex : vertexes)
			out << xpsHelper.GetWellCoords(vertex).first << " "
			<< xpsHelper.GetWellCoords(vertex).second << "\n";

		out << "\n";
	}
}

int main(int argc, char* argv[]) {
	TimeChecker checkTime;
	XPSHelper xps(argv[1]);
//	G3 g3(xps, 4);

	KLines klines(xps);
	auto blockPath = klines.CreateBlockPath(13, KLineDirection::DOWN_VERTICAL, KLineFrom::FROM_LEFT);

/*
	for (int i = 0; i < blockPath.size(); ++i) {
		for (int j = 0; j < blockPath[i].size(); ++j)
			std::cout << blockPath[i][j] << " ";
		std::cout << std::endl;
	}

	std::ofstream out("vis.txt", std::ios::out);
	PathHelper ph(g3);

	out << g3.GetXPSHelper().GetWellCount() << "\n";
	for (int i = 0; i < g3.GetXPSHelper().GetWellCount(); ++i)
		out << g3.GetXPSHelper().GetWellCoords(i).first << " " << g3.GetXPSHelper().GetWellCoords(i).second << "\n";
	out << "\n";

	ph.FindPath(std::vector<int>{0, 1, 2, 3});
	ph.Optimize();

	ph.WritePath(out);
	out.close();
*/
	std::ofstream out("visLeha.txt", std::ios::out);
	WriteXPSPath(blockPath, xps, out);
	out.close();

	out.open("vis.txt", std::ios::out);
	WritePath(blockPath, xps, out);
	out.close();

/*
	out.open("visLeha.txt", std::ios::out);
	ph.WriteXPSPath(out);
	out.close();
*/

	return 0;
}