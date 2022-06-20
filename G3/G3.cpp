#include "G3.hpp"

#include <set>
#include <iostream>

G3::G3(const XPSHelper& hlp, int k) :
	xpsHlp_(hlp),
	K_(k)
{
	std::cout << "Start G3 Constructor" << std::endl;

	std::set<std::pair<int, int>> wellsCoords;
	for (int i = 0; i < xpsHlp_.GetWellCount(); ++i)
		wellsCoords.insert(xpsHlp_.GetWellCoords(i));

	for (int i = 1; i <= k; ++i)
		for (int j = i; j <= k; ++j) {
			if (i * j > k)
				continue;

			AddWellsField(i, j, wellsCoords);
			if (i != j)
				AddWellsField(j, i, wellsCoords);
		}

	UniqueField();
	SetCoats();

	G3VertexInG1Vertex_.resize(xpsHlp_.GetWellCount());
	for (int i = 0; i < vertex_.size(); ++i)
		for (int g1Vertex : vertex_[i])
			G3VertexInG1Vertex_[g1Vertex].push_back(i);

	SetAdjList();

	vertexCoord_.resize(vertex_.size());
	for (int i = 0; i < vertexCoord_.size(); ++i) {
		for (int j = 0; j < vertex_[i].size(); ++j) {
			vertexCoord_[i].push_back(xpsHlp_.GetWellCoords(vertex_[i][j]));
		}
	}

	for (int i = 0; i < vertexCoord_.size(); ++i)
		coordVertex_[vertexCoord_[i]] = i;
}

std::vector<std::pair<int, int>> Intersection(std::vector<std::pair<int, int>> form, const std::set<std::pair<int, int>> wellsCoords) {
	std::vector<std::pair<int, int>> ret;
	for (const auto& coords : form)
		if (wellsCoords.count(coords))
			ret.push_back(coords);

	return ret;
}

void G3::AddWellsField(int w, int h, const std::set<std::pair<int, int>> wellsCoords) {
	int maxX = 0,
		maxY = 0;

	for (const auto& coords : wellsCoords) {
		maxX = std::max(maxX, coords.first);
		maxY = std::max(maxY, coords.second);
	}

	std::vector<std::pair<int, int>> form;
	for (int i = 0; i < w; ++i)
		for (int j = 0; j < h; ++j)
			form.push_back({-i, -j});

	while (Intersection(form, wellsCoords).size() > 0 or form.front().second <= maxY) {
		auto xForm = form;
		while (Intersection(xForm, wellsCoords).size() > 0 or xForm.front().first <= maxX) {
			if (Intersection(xForm, wellsCoords).size() > 0) {
				auto inters = Intersection(xForm, wellsCoords);
				std::vector<int> new_vertex;

				for (const auto& coords : inters)
					new_vertex.push_back(xpsHlp_.GetWellNum(coords));

				std::sort(new_vertex.begin(), new_vertex.end());

				vertex_.push_back(new_vertex);
			}

			for (auto& coords : xForm)
				coords.first++;
		}

		for (auto& coords : form)
			coords.second++;
	}

	std::cout << "Add Wells Field " << w << " " << h << std::endl;
}

void G3::UniqueField() {
	std::set<std::vector<int>> setVertex;
	std::vector<std::vector<int>> newVertex;

	for (const auto& vertex : vertex_) {
		if (not setVertex.count(vertex)) {
			newVertex.push_back(vertex);
			setVertex.insert(vertex);
		}
	}

	vertex_ = newVertex;

	std::cout << "Unique Field OK" << std::endl;
}

void G3::SetCoats() {
	for (int i = 0; i < vertex_.size(); ++i) {
		RArea cur;

		for (const auto& wellNum : vertex_[i])
			cur = Merge(cur, xpsHlp_.GetWellCoat(wellNum));

		coat_.push_back(cur);
	}

	std::cout << "Set Coats OK" << std::endl;
}

void G3::SetAdjList() {
	adjList_.resize(vertex_.size());

	std::vector<std::pair<int, int>> dirs = {{0, 1}, {0, -1}, {1, 0}, {-1, 0}};
	std::set<std::pair<int, int>> wellsCoords;
	for (int i = 0; i < xpsHlp_.GetWellCount(); ++i)
		wellsCoords.insert(xpsHlp_.GetWellCoords(i));

	for (int i = 0; i < adjList_.size(); ++i) {
		std::set<int> edgeEst;
		std::set<int> curG3VertexEst;
		for (int g1Vertex : vertex_[i])
			curG3VertexEst.insert(g1Vertex);
		
		for (int g1Vertex : vertex_[i]) {
			std::pair<int, int> g1VertexCoord = xpsHlp_.GetWellCoords(g1Vertex);

			for (auto dir : dirs) {
				auto g3NVertexCoord = g1VertexCoord;
				g3NVertexCoord.first += dir.first;
				g3NVertexCoord.second += dir.second;

				if (wellsCoords.count(g3NVertexCoord)) {
					int g1NVertex = xpsHlp_.GetWellNum(g3NVertexCoord);
					for (int g3Vertex : G3VertexInG1Vertex_[g1NVertex])
						edgeEst.insert(g3Vertex);
				}
			}
		}

		for (int g3Vertex : edgeEst) {
			bool doAdd = true;
			for (int g1Vertex : vertex_[g3Vertex])
				if (curG3VertexEst.count(g1Vertex)) {
					doAdd = false;
					break;
				}

			if (doAdd)
				adjList_[i].push_back(g3Vertex);
		}

		//std::cout << adjList_[i].size() << std::endl;
	}

/*
	for (int i = 0; i < vertex_.size(); ++i)
		for (int j = i + 1; j < vertex_.size(); ++j)
			if (IntersectionSize(coat_[i], coat_[j]) > std::max(coat_[i].Square(), coat_[j].Square()) / 2) {
				adjList_[i].push_back(j);
				adjList_[j].push_back(i);
			}
*/
	std::cout << "Set Adj List OK" << std::endl;
}

const std::vector<std::vector<int>>& G3::GetVertex() const {
	return vertex_;
}

const std::vector<std::vector<int>>& G3::GetAdjList() const {
	return adjList_;
}

const std::vector<RArea>& G3::GetCoat() const {
	return coat_;
}

const int G3::GetK() const {
	return K_;
}

const XPSHelper& G3::GetXPSHelper() const {
	return xpsHlp_;
}

const std::vector<std::pair<int, int>>& G3::GetVertexCoord(int vertex) const {
	return vertexCoord_[vertex];
}

int G3::GetVertexNum(std::vector<std::pair<int, int>> vertexCoord) const {
	return coordVertex_.at(vertexCoord);
}