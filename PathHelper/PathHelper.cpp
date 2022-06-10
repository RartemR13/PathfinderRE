#include "PathHelper.hpp"

#include <stdexcept>
#include <algorithm>
#include <set>

void dfs(int v,
	const std::vector<std::vector<int>>& g,
	std::vector<int>& order) {
	order.push_back(v);

	for(auto to : g[v]) {
		dfs(to, g, order);
		order.push_back(v);
	}
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

void PathHelper::GenQue(int num, std::vector<std::vector<int>>& toQue, const std::set<int>& toCheck) {
	for (int to : g3_.GetAdjList()[num])
		if (not IsInters(to, toCheck))
			toQue[g3_.GetVertex()[to].size()].push_back(to);

	for (int i = 0; i < toQue.size(); ++i)
		std::sort(toQue[i].begin(), toQue[i].end(),
		[&](int lhs, int rhs) {
			return IntersectionSize(g3_.GetCoat()[num], g3_.GetCoat()[lhs]) > IntersectionSize(g3_.GetCoat()[num], g3_.GetCoat()[rhs]);	
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
	while (curField.size() < g3_.GetXPSHelper().GetWellCount()) {
		int next = -1;

		for (int i = que.size() - 1; i > 0; --i) {
			if (que[i].size() == 0)
				continue;

			std::cout << curField.size() << "/" << g3_.GetXPSHelper().GetWellCount() << "\n"; 

			next = que[i].front();
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

	///
}

const std::vector<std::vector<int>>& PathHelper::GetPath() {
	return path_;
}