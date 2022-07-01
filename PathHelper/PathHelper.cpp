#include "PathHelper.hpp"

#include <stdexcept>
#include <algorithm>
#include <set>
#include <windows.h>

std::vector<std::pair<int, int>> PathHelper::GetVertexGeometry(int vertex) {
	std::vector<std::pair<int, int>> ret;

	for (const auto well : g3_.GetVertex()[vertex])
		ret.push_back(g3_.GetXPSHelper().GetWellCoords(well));

	return ret;
}

bool IsHorisontal(Direction d) {
	return d == Direction::RIGHT || d == Direction::LEFT;
}

bool IsVertical(Direction d) {
	return d == Direction::DOWN || d == Direction::UP;
}

bool IsPerpendicular(Direction d1, Direction d2) {
	return IsVertical(d1) && IsHorisontal(d2) || IsVertical(d2) && IsHorisontal(d1);
}

Direction Opposite(Direction d) {
	if(IsVertical(d))
		return d == Direction::DOWN ? Direction::UP : Direction::DOWN;
	return d == Direction::RIGHT ? Direction::LEFT : Direction::RIGHT;
}

Direction PathHelper::GetDirection(int lhs, int rhs) {
	auto lhsGeom = GetVertexGeometry(lhs),
		 rhsGeom = GetVertexGeometry(rhs);

	std::sort(lhsGeom.begin(), lhsGeom.end());
	std::sort(rhsGeom.begin(), rhsGeom.end());

	int lower_boundl = lhsGeom[0].second;
	int upper_boundl = lhsGeom[lhsGeom.size() - 1].second;
	int left_boundl = lhsGeom[0].first;
	int right_boundl = lhsGeom[lhsGeom.size() - 1].first;

	int lower_boundr = rhsGeom[0].second;
	int upper_boundr = rhsGeom[rhsGeom.size() - 1].second;
	int left_boundr = rhsGeom[0].first;
	int right_boundr = rhsGeom[rhsGeom.size() - 1].first;

	if(lower_boundl <= lower_boundr && upper_boundl >= upper_boundr || 
		lower_boundl >= lower_boundr && upper_boundl <= upper_boundr) {
		if(right_boundl <= left_boundr) {
			return Direction::RIGHT;
		}
		else if (left_boundl >= right_boundr) {
			return Direction::LEFT;
		}
	}
	else if(left_boundl <= left_boundr && right_boundl >= right_boundr ||
		left_boundl >= left_boundr && right_boundl <= right_boundr) {
		if(lower_boundl >= upper_boundr) {
			return Direction::DOWN;
		}
		else if(upper_boundl <= lower_boundr) {
			return Direction::UP;
		}
	}
	return Direction::RANDOM;
}

std::pair<int, int> PathHelper::Size(int v) {
	auto geom = GetVertexGeometry(v);
	std::sort(geom.begin(), geom.end());
	int lower_bound = geom[0].second;
	int upper_bound = geom[geom.size() - 1].second;
	int left_bound = geom[0].first;
	int right_bound = geom[geom.size() - 1].first;
	return std::make_pair(right_bound - left_bound + 1, upper_bound - lower_bound + 1);
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
		});
}

int PathHelper::GetNext(std::set<int>& curField, std::vector<std::vector<int>>& que, std::vector<int>& p,
						int& last, Direction& lastDirection) {
	int next = -1;
	bool canGoForw = false;
	bool isUturn = false;
	std::vector<int> candidate;

	//пытаемся пойти вперед и не сильно изменить форму
	for (int i = que.size() - 1; i > 0; --i) {
		if(que[i].empty())
			continue;
		for (const auto& vertex : que[i]) {
			if (GetDirection(last, vertex) == lastDirection) {
				canGoForw = true;
				auto lastSz = Size(last);
				auto vertexSz = Size(vertex);
				if (IsHorisontal(lastDirection) && 
				   (lastSz.second <= vertexSz.second || 
					lastSz.second >= vertexSz.second && 1.0 * lastSz.second / vertexSz.second <= 2) ||
					IsVertical(lastDirection) &&
					(lastSz.first <= vertexSz.first ||
						lastSz.first >= vertexSz.first && 1.0 * lastSz.first / vertexSz.first <= 2)) {
						candidate.push_back(vertex);
				}
			}
		}
	}

	if (!candidate.empty()) {
		std::sort(candidate.begin(), candidate.end(), [&](int lhs, int rhs) {
			if (IsVertical(lastDirection))
				return Size(lhs).first > Size(rhs).first;
			return Size(lhs).second > Size(rhs).second;
			});
		next = candidate[0];
	}

	//уперлись в стенку и можем развернуться
	if (next == -1 && !canGoForw) {
		candidate.clear();
		for (int i = que.size() - 1; i > 0; --i) {
			if(que[i].empty())
				continue;

			for (const auto& vertex : que[i]) {
				if (IsPerpendicular(lastDirection, GetDirection(last, vertex))) {
					candidate.push_back(vertex);
				}
			}
		}

		if(!candidate.empty()) {
			isUturn = true;
			std::sort(candidate.begin(), candidate.end(), [&](int lhs, int rhs) {
				if(Size(last).first > Size(last).second)
					return Size(lhs).first > Size(rhs).first;
				return Size(lhs).second > Size(rhs).second;
				});
			next = candidate[0];
		}
	}

	//можем пойти вперед но сильно меняем форму, или не вперед
	if (next == -1) {
		for (int i = que.size() - 1; i > 0; --i) {
			if(que[i].empty())
				continue;
			next = que[i][0];
			candidate.clear();
			for(const auto& vertex : que[i])
				if(SimSum(last, next) == SimSum(last, vertex) &&
					VertexEqu(last, vertex)) {
					candidate.push_back(vertex);
				}

			if(!candidate.empty()) {
				next = candidate[0];
				if(lastDirection != Direction::RANDOM) {
					for(const auto& vertex : candidate) {
						if(VertexEqu(last, vertex) && GetDirection(last, vertex) == lastDirection) {
							next = vertex;
							break;
						}
					}
				}
			}

			if(next != -1) {
				break;
			}
		}
	}

	//если не нашли адекватного дитя идем к предку-предку
	if(next == -1) {
		candidate.clear();
		int cur = p[last];
		if(cur == last)
			throw std::runtime_error("Bad dads");

		for(int i = 0; i < que.size(); ++i)
			que[i].clear();

		GenQue(cur, que, curField);
		last = cur;
		lastDirection = GetDirection(p[last], last);
		return GetNext(curField, que, p, last, lastDirection);
	}

	if(!isUturn && next != -1) {
		lastDirection = GetDirection(last, next);
	}
	else {
		lastDirection = Opposite(lastDirection);
	}

	AddField(next, curField);
	p[next] = last;
	
	std::cout << curField.size() << "/" << g3_.GetXPSHelper().GetWellCount() << std::endl;
	last = next;
	
	for(int i = 0; i < que.size(); ++i)
		que[i].clear();
	GenQue(next, que, curField);
	return next;
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
	Direction lastDirection = Size(last).first <= Size(last).second ? Direction::RIGHT : Direction::DOWN;

	while (curField.size() < g3_.GetXPSHelper().GetWellCount()) {
		path_.push_back(GetNext(curField, que, p, last, lastDirection));
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

	for(const auto& vertex : path_) {
		const auto& vertexes = g3_.GetVertex()[vertex];
		out << vertexes.size() << " 0" << "\n";
		for(const auto& vertex : vertexes)
			out << g3_.GetXPSHelper().GetWellCoords(vertex).first << " "
			<< g3_.GetXPSHelper().GetWellCoords(vertex).second << "\n";

		out << "\n";
	}
}

void PathHelper::Optimize() {
	std::ofstream out("tsp_file.tsp", std::ios::out);

	out << "DIMENSION : " << path_.size() + 1<< "\n";
	out << "EDGE_WEIGHT_TYPE : EXPLICIT\n";
	out << "EDGE_WEIGHT_FORMAT : FULL_MATRIX\n";
	out << "EDGE_WEIGHT_SECTION\n";

	int max_edge = 0;
	for(int i = 0; i < path_.size(); ++i){
		for(int j = i + 1; j < path_.size(); ++j){
			max_edge = std::max(max_edge, SimSum(path_[i], path_[j]));
		}
	}

	for (int i = 0; i < path_.size(); ++i) {
		for (int j = 0; j < path_.size(); ++j) {
			if (i == j) {
				out << INT_MAX << " ";
				continue;
			}
			out << SimSum(path_[i], path_[j]) << " ";
		}
		out << max_edge + 1 << " "; //фейковая ноду
		out << "\n";
	}
	for(int i = 0; i < path_.size(); ++i) {
		out << max_edge + 1 << " "; //фейковая ноду
	}
	out << INT_MAX << "\n";

	out << "EOF\n";
	out.close();

	STARTUPINFO StartupInfo;
	ZeroMemory(&StartupInfo, sizeof(StartupInfo));
	StartupInfo.cb = sizeof(StartupInfo);

	PROCESS_INFORMATION ProcInfo;
	ZeroMemory(&ProcInfo, sizeof(ProcInfo));
	if (not CreateProcess(NULL,
						  "VoyagerSolver\\LinKernighan.exe -o in.txt tsp_file.tsp",
						  NULL,
						  NULL,
						  FALSE,
						  0,
						  NULL,
						  NULL,
						  &StartupInfo,
						  &ProcInfo)) 
	{}

	WaitForSingleObject(ProcInfo.hProcess, INFINITE);
	CloseHandle(ProcInfo.hProcess);
	CloseHandle(ProcInfo.hThread);

	std::ifstream in("in.txt", std::ios::in);

	int t;
	in >> t >> t;

	std::vector<std::vector<int>> edges;

	for (int i = 0; i < path_.size() + 1; ++i) {
		int v, to, weight;
		in >> v >> to >> weight;
		edges.push_back({v, to, weight});
	}
	in.close();

	int fakeEdgeFromNum = -1,
		fakeEdgeToNum = -1;

	for (int i = 0; i < edges.size(); ++i){
		if (edges[i][1] == path_.size()) {
			fakeEdgeToNum = i; 
		}
		if (edges[i][0] == path_.size()) {
			fakeEdgeFromNum = i; 
		}
	}

	auto oldPath = path_;

	path_.clear();

	if (fakeEdgeToNum < fakeEdgeFromNum){
		for (int i = fakeEdgeFromNum + 1; i < edges.size(); ++i) {
			if (path_.empty() or path_.back() != edges[i][0])
				path_.push_back(edges[i][0]);
			if (path_.empty() or path_.back() != edges[i][1])
				path_.push_back(edges[i][1]);

		}
		for (int i = 0; i < fakeEdgeToNum; ++i) {
			if (path_.empty() or path_.back() != edges[i][0])
				path_.push_back(edges[i][0]);
			if (path_.empty() or path_.back() != edges[i][1])
				path_.push_back(edges[i][1]);
		}	
	}
	else{
		for (int i = fakeEdgeFromNum + 1; i < fakeEdgeToNum; ++i) {
			if (path_.empty() or path_.back() != edges[i][0])
				path_.push_back(edges[i][0]);
			if (path_.empty() or path_.back() != edges[i][1])
				path_.push_back(edges[i][1]);
		}
	}

	for (int i = 0; i < path_.size(); ++i)
		path_[i] = oldPath[path_[i]];
}

const std::vector<int>& PathHelper::GetPath() {
	return path_;
}

void PathHelper::WriteXPSPath(std::ofstream& out) {
    if(not out.is_open())
        throw std::invalid_argument("\n"
            "Class PathHelper\n"
            "Method WriteXPSPath\n"
            "Output file at output stream is not open\n"
        );

    int blocknum = 0;
    for(const auto& vertex : path_) {
        const auto& vertexes = g3_.GetVertex()[vertex];
        for(const auto& vertex : vertexes)
            out << g3_.GetXPSHelper().GetOldCoords(vertex).first << " "
            << g3_.GetXPSHelper().GetOldCoords(vertex).second << " " << blocknum << "\n";
        blocknum++;
    }
}