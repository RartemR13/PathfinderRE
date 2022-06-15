#include "PathHelper.hpp"

#include <stdexcept>
#include <algorithm>
#include <set>

std::vector<std::pair<int, int>> PathHelper::GetVertexGeometry(int vertex) {
	std::vector<std::pair<int, int>> ret;

	for (const auto well : g3_.GetVertex()[vertex])
		ret.push_back(g3_.GetXPSHelper().GetWellCoords(well));

	return ret;
}

Direction PathHelper::GetDirection(int lhs, int rhs) {
	if (not VertexEqu(lhs, rhs))
		return Direction::RANDOM;

	auto lhsGeom = GetVertexGeometry(lhs),
		 rhsGeom = GetVertexGeometry(rhs);

	std::sort(lhsGeom.begin(), lhsGeom.end());
	std::sort(rhsGeom.begin(), rhsGeom.end());

	std::vector<std::pair<int, int>> GeomD;
	for (int i = 0; i < rhsGeom.size(); ++i)
		GeomD.push_back(std::make_pair(rhsGeom[i].first - lhsGeom[i].first, rhsGeom[i].second - lhsGeom[i].second));

	for (int i = 1; i < GeomD.size(); ++i)
		if (GeomD[i] != GeomD[i-1])
			return Direction::RANDOM;

	std::pair<int, int> curD = GeomD.front();

	if (curD == std::make_pair(1, 0))
		return Direction::RIGHT;
	else if (curD == std::make_pair(-1, 0))
		return Direction::LEFT;
	else if (curD == std::make_pair(0, 1))
		return Direction::UP;
	else if (curD == std::make_pair(0, -1))
		return Direction::DOWN;

	return Direction::RANDOM;
}

bool PathHelper::VertexEqu(int lhs, int rhs) {
	auto lhsGeom = GetVertexGeometry(lhs),
		 rhsGeom = GetVertexGeometry(rhs);

	if (lhsGeom.size() != rhsGeom.size())
		return false;

	std::sort(lhsGeom.begin(), lhsGeom.end());
	std::sort(rhsGeom.begin(), rhsGeom.end());

	int lhsDx = lhsGeom.front().first,
		lhsDy = lhsGeom.front().second;

	for (int i = 0; i < lhsGeom.size(); ++i) {
		lhsGeom[i].first  -= lhsDx;
		lhsGeom[i].second -= lhsDy;
	}

	int rhsDx = rhsGeom.front().first,
		rhsDy = rhsGeom.front().second;

	for (int i = 0; i < rhsGeom.size(); ++i) {
		rhsGeom[i].first  -= rhsDx;
		rhsGeom[i].second -= rhsDy;
	}

	return lhsGeom == rhsGeom;
}

PathHelper::PathHelper(const G3& g3) : 
	g3_(g3) 
{}

int PathHelper::FindVertex(std::vector<int> st) {
	for (int i = 0; i < g3_.GetVertex().size(); ++i)
		if (g3_.GetVertex()[i] == st)
			return i;

	throw std::runtime_error("Can not find vertex");
}

void PathHelper::AddField(int num, std::set<int>& toAdd) {
	for (int i = 0; i < g3_.GetVertex()[num].size(); ++i)
		toAdd.insert(g3_.GetVertex()[num][i]);
}

bool PathHelper::IsInters(int num, const std::set<int>& toCheck) {
	for (int i = 0; i < g3_.GetVertex()[num].size(); ++i)
		if (toCheck.count(g3_.GetVertex()[num][i]))
			return true;

	return false;
}

int PathHelper::SimSum(int lhs, int rhs) {
	return g3_.GetCoat()[lhs].Square() + g3_.GetCoat()[rhs].Square() - 2 * IntersectionSize(g3_.GetCoat()[lhs], g3_.GetCoat()[rhs]);
}

void PathHelper::GenQue(int num, std::vector<std::vector<int>>& toQue, const std::set<int>& toCheck) {
	for (int to : g3_.GetAdjList()[num])
		if (not IsInters(to, toCheck))
			toQue[g3_.GetVertex()[to].size()].push_back(to);

	for (int i = 0; i < toQue.size(); ++i)
		std::sort(toQue[i].begin(), toQue[i].end(),
		[&](int lhs, int rhs) {
			return SimSum(num, lhs) < SimSum(num, rhs);
/*
			return g3_.GetCoat()[num].Square() + g3_.GetCoat()[lhs].Square() - IntersectionSize(g3_.GetCoat()[num], g3_.GetCoat()[lhs]) < 
				   g3_.GetCoat()[num].Square() + g3_.GetCoat()[rhs].Square() - IntersectionSize(g3_.GetCoat()[num], g3_.GetCoat()[rhs]);
*/	
		});
}

void PathHelper::FindPath(std::vector<int> st) {
	std::cout << "Start Find Path" << std::endl;

	int stNum = FindVertex(st);

	std::cout << "Vertex Num: " << stNum << std::endl;

	std::set<int> curField;
	std::vector<std::vector<int>> que(g3_.GetK() + 1);
	std::vector<int> p(g3_.GetVertex().size(), -1);

	que[st.size()].push_back(stNum);

	int last = stNum;
	Direction lastDirection = Direction::RANDOM;

	while (curField.size() < g3_.GetXPSHelper().GetWellCount()) {
		int next = -1;

		if (g3_.GetVertex()[last] == std::vector<int>{4104, 4178}) {
			for (int i = 0; i < que.size(); ++i) {
				std::cout << "que[" << i << "]: " << que[i].size() << std::endl;
				for (int j = 0; j < que[i].size(); ++j) {
					for (int v : g3_.GetVertex()[que[i][j]])
						std::cout << v << " ";
					std::cout << "\n";

					std::cout << "Sim Sum: " << SimSum(last, que[i][j]) << std::endl << std::endl;
				} 
			}
		}

		for (int i = que.size() - 1; i > 0; --i) {
			if (que[i].size() == 0)
				continue;

			std::cout << curField.size() << "/" << g3_.GetXPSHelper().GetWellCount() << "\n"; 
			next = que[i].front();

			std::vector<int> newNextPret;
			for (const auto& vertex : que[i])
				if (SimSum(last, next) == SimSum(last, vertex) and
					VertexEqu(last, vertex))
				{
					newNextPret.push_back(vertex);
				}

			if (newNextPret.size() > 0) {
				next = newNextPret.front();

				std::cout << "PADLA " << g3_.GetVertex()[next][0] << " " << g3_.GetVertex()[next][0] << std::endl; 

				if (lastDirection != Direction::RANDOM) {
					for (const auto& vertex : newNextPret) {
						if (GetDirection(last, vertex) == lastDirection) {
							next = vertex;
							break;
						}
					}
				}
			}


			std::cout << g3_.GetAdjList()[next].size() << std::endl;

			break;
		}

		if (next == -1) {

			int cur = p[last];

			std::cout << "! " << last << std::endl;

			if (cur == last)
				throw std::runtime_error("Bad dads");

			for (int i = 0; i < que.size(); ++i)
				que[i].clear();

			GenQue(cur, que, curField);
			last = cur;
			continue;
		}

		std::cout << "next " << next << std::endl;

		AddField(next, curField);
		p[next] = last;
		lastDirection = GetDirection(last, next);
		std::cout << "Direction " << lastDirection << std::endl;
		last = next;
		path_.push_back(g3_.GetVertex()[next]);

		for (int i = 0; i < que.size(); ++i)
			que[i].clear();

		GenQue(next, que, curField);
	}
}

void PathHelper::WritePath(std::ofstream& out) {
	if(not out.is_open())
		throw std::invalid_argument("\n"
			"Class PathHelper\n"
			"Method WritePath\n"
			"Output file at output stream is not open\n"
		);

	out << path_.size() << "\n\n";

	for(const auto& vertexes : path_) {
		out << vertexes.size() << " 0" << "\n";
		for(const auto& vertex : vertexes)
			out << g3_.GetXPSHelper().GetWellCoords(vertex).first << " "
			<< g3_.GetXPSHelper().GetWellCoords(vertex).second << "\n";

		out << "\n";
	}
}

void PathHelper::WriteXPSPath(std::ofstream& out) {
	if(not out.is_open())
		throw std::invalid_argument("\n"
			"Class PathHelper\n"
			"Method WriteXPSPath\n"
			"Output file at output stream is not open\n"
		);
}

const std::vector<std::vector<int>>& PathHelper::GetPath() {
	return path_;
}