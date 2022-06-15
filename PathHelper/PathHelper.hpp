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
	const std::vector<std::vector<int>>& GetPath();
	//возращает последовательность наборов единичных вершин

private:
	int FindVertex(std::vector<int> st);
	void AddField(int num, std::set<int>& toAdd);
	bool IsInters(int num, const std::set<int>& toCheck);
	void GenQue(int num, std::vector<std::vector<int>>& toQue, const std::set<int>& toCheck);

	int SimSum(int lhs, int rhs);
	bool VertexEqu(int lhs, int rhs);
	Direction GetDirection(int lhs, int rhs);
	std::vector<std::pair<int, int>> GetVertexGeometry(int vertex);

	std::vector<std::vector<int>> path_;
	const G3& g3_;
};