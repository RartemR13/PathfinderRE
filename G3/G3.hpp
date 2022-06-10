#pragma once

#include <vector>
#include <map>

#include "../RArea/RArea.hpp"
#include "../XPSHelper/XPSHelper.hpp"

class G3 {
public:
	G3(const XPSHelper& hlp, int k);
	const std::vector<std::pair<int, int>>& GetVertexOldCoord() const;
	const std::vector<std::pair<int, int>>& GetVertexCoord() const;

	const std::vector<std::vector<int>>& GetVertex() const;
	const std::vector<std::vector<int>>& GetAdjList() const;
	const std::vector<RArea>& GetCoat() const;
	const int GetK() const;
	const XPSHelper& GetXPSHelper() const;

private:
	void AddWellsField(int w, int h, const std::set<std::pair<int, int>> wellsCoords);
	void UniqueField();
	void SetCoats();
	void SetAdjList();

	std::vector<std::vector<int>> vertex_; 
	//вектор наборов единичных вершин, Vertex[i] - набор единичных вершин
	std::vector<RArea> coat_;
	//покрытия наборов вершин, Coat[i] - Rarea для i-ого набора Vertex
	std::vector<std::vector<int>> adjList_;
	//список смежности между наборами вершин
	const XPSHelper& xpsHlp_;
	const int K_;
};