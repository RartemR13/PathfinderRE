#pragma once

#include "../G3/G3.hpp"

#include <string>
#include <fstream>
#include <vector>

enum Direction {
	LEFT,
	RIGHT,
	UP,
	DOWN,
	RANDOM
};

class PathHelper {
public:
	PathHelper(const G3& g3);
	
	void FindPath(std::vector<int> st);
	void WritePath(std::ofstream& out);
	void WriteXPSPath(std::ofstream& out);
	const std::vector<int>& GetPath();
	//возращает последовательность наборов единичных вершин

	void Optimize();

	int GetNext(std::set<int>& curField, std::vector<std::vector<int>>& que, std::vector<int>& p,
				int& last, Direction& lastDirection);

	int GetPathSize();
	void SimAnn(double startTemp, double speed, double minTemp, std::vector<int> st, bool pathEst);

	void WriteG3Path(std::ofstream& out);

	void ReadG3Path(std::ifstream& in);

private:
	int FindVertex(std::vector<int> st);
	void AddField(int num, std::set<int>& toAdd);
	bool IsInters(int num, const std::set<int>& toCheck);
	void GenQue(int num, std::vector<std::vector<int>>& toQue, const std::set<int>& toCheck);

	int SimSum(int lhs, int rhs);
	bool VertexEqu(int lhs, int rhs);
	Direction GetDirection(int lhs, int rhs);
	std::vector<std::pair<int, int>> GetVertexGeometry(int vertex);

	std::pair<int, int> Size(int v);

	std::vector<int> path_;
	const G3& g3_;
};