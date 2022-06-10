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
	SetAdjList();
/*
	std::cout << "G3 Constructor OK" << std::endl;
	for (int i = 0; i < vertex_.size(); ++i) {
		for (auto it : vertex_[i])
			std::cout << it << " ";
		std::cout << std::endl;
	}
*/
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
		//std::cout << cur.Square() << std::endl;
	}

	std::cout << "Set Coats OK" << std::endl;
}

void G3::SetAdjList() {
	adjList_.resize(vertex_.size());
	for (int i = 0; i < vertex_.size(); ++i)
		for (int j = i + 1; j < vertex_.size(); ++j)
			if (IntersectionSize(coat_[i], coat_[j]) > 0) {
				adjList_[i].push_back(j);
				adjList_[j].push_back(i);
			}

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

/*
const std::vector<std::pair<int, int>>& G3::GetVertexOldCoord() const {
	return XPSoldGeom_;
}
*/
/*
const std::vector<std::pair<int, int>>& G3::GetVertexCoord() const {
	return XPSgeom_;
}
*/