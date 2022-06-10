#include "RArea.hpp"

#include <cassert>
#include <iostream>
#include <algorithm>
//#include <deque>
namespace bg  = boost::geometry;
namespace bgm = bg::model;

using point = bgm::d2::point_xy<int>;
using polygon = bgm::polygon<point>;
using multi_polygon = bgm::multi_polygon<polygon>;

RArea::RArea(const std::vector<std::string>& data) {
	std::vector<std::vector<std::pair<int, int>>> points;
	for (int i = 0; i < data.size(); ++i) {
		std::string temp = data[i].substr(63, 16);
		int line = std::stoi(temp.substr(0, 4));
		int l = std::stoi(temp.substr(4, 4));
		int r = std::stoi(temp.substr(12, 4));

		points.push_back({std::make_pair(l, line), std::make_pair(r, line)});
	}

	std::sort(points.begin(), points.end(), 
	[](std::vector<std::pair<int, int>> lhs,
	   std::vector<std::pair<int, int>> rhs) {

		return lhs[0].second < rhs[0].second;
	});

	std::vector<std::pair<int, int>> n_data;
	std::vector<int> n_lines;

	for (int i = 0; i < points.size(); ++i) {
		n_data.push_back(std::make_pair(points[i][0].first, points[i][1].first));
		n_lines.push_back(points[i][0].second);
	}

	storage_ = RArea(n_data, n_lines).GetStorage();

/*
	std::vector<std::vector<std::pair<int, int>>> points[4];

	for (int i = 0; i < data.size(); ++i) {
		std::string temp = data[i].substr(63, 16);
		int line = std::stoi(temp.substr(0, 4));
		int l = std::stoi(temp.substr(4, 4));
		int r = std::stoi(temp.substr(12, 4));

		points[0].push_back({std::make_pair(l, line - 1), std::make_pair(r, line - 1)});
		points[1].push_back({std::make_pair(l, line), std::make_pair(r, line)});
		points[2].push_back({std::make_pair(l - 1, line), std::make_pair(r - 1, line)});
		points[3].push_back({std::make_pair(l - 1, line - 1), std::make_pair(r - 1, line - 1)});
	}

	for (int i = 0; i < 4; ++i) {
		std::sort(points[i].begin(), points[i].end(), 
		[](std::vector<std::pair<int, int>> lhs,
		   std::vector<std::pair<int, int>> rhs) {

			return lhs[0].second < rhs[0].second;
		});
		std::reverse(points[i].begin(), points[i].end());
	}

	polygon cur[4];

	for (int i = 0; i < 4; ++i) {
		bg::append(cur[i], point(points[i][0][0].first, points[i][0][0].second));
		bg::append(cur[i], point(points[i][0][1].first, points[i][0][1].second));

		for (int j = 1; j < points[i].size(); ++j)
			bg::append(cur[i], point(points[i][j][1].first, points[i][j][1].second));

		for (int j = points[i].size() - 1; j >= 0; --j)
			bg::append(cur[i], point(points[i][j][0].first, points[i][j][0].second));
	}

	multi_polygon res = {cur[0]}, temp;

	for (int i = 1; i < 4; ++i) {
		bg::union_(res, cur[i], temp);
		res = temp;
		temp.clear();
	}

	storage_ = res;
*/
}

RArea::RArea(const std::vector<std::pair<int, int>>& data, const std::vector<int>& lines) {
	std::vector<std::vector<std::pair<int, int>>> points;

	for (int i = 0; i < data.size(); ++i) {
		points.push_back({std::make_pair(data[i].first, lines[i]), std::make_pair(data[i].second + 1, lines[i])});
		points.push_back({std::make_pair(data[i].first, lines[i] + 1), std::make_pair(data[i].second + 1, lines[i] + 1)});
	}

	polygon cur;

	//bg::append(cur, point(points[0][0].first, points[0][0].second));
	//bg::append(cur, point(points[0][1].first, points[0][1].second));

	for (int i = 0; i < points.size(); ++i)
		bg::append(cur, point(points[i][0].first, points[i][0].second));

	//bg::append(cur, point(points[points.size()-1][1].first, points[points.size()-1][1].second));

	for (int i = static_cast<int>(points.size()) - 1; i >= 0; --i)
		bg::append(cur, point(points[i][1].first, points[i][1].second));

	if (points.size() > 0)
		bg::append(cur, point(points[0][0].first, points[0][0].second));

	storage_ = {cur};
	Compress();
	bg::correct(storage_);
}

std::vector<std::pair<int, int>> RArea::GetVertexPath() {
	std::vector<std::pair<int, int>> ret;

	for (auto pol_it = boost::begin(storage_); pol_it != boost::end(storage_); ++pol_it)
		for (auto pnt_it = boost::begin(bg::exterior_ring(*pol_it)); pnt_it != boost::end(bg::exterior_ring(*pol_it)); ++pnt_it)
			ret.push_back(std::make_pair(bg::get<0>(*pnt_it), bg::get<1>(*pnt_it)));

	return ret;
}

void RArea::Compress() {
	auto vertex_path = GetVertexPath();
	if (vertex_path.size() < 2)
		return;

	bool napr = (vertex_path[0].first != vertex_path[1].first);

	std::vector<std::pair<int, int>> res_path = {vertex_path[0], vertex_path[1]};
	for (int i = 2; i < vertex_path.size(); ++i) {
		if (napr) {
			if (res_path.back().first != vertex_path[i].first) {
				res_path.pop_back();
				res_path.push_back(vertex_path[i]);
				continue;
			}

			if (res_path.back().second != vertex_path[i].second) {
				napr = not napr;
				res_path.push_back(vertex_path[i]);
			}
		} else {
			if (res_path.back().first != vertex_path[i].first) {
				napr = not napr;
				res_path.push_back(vertex_path[i]);
				continue;
			}

			if (res_path.back().second != vertex_path[i].second) {
				res_path.pop_back();
				res_path.push_back(vertex_path[i]);
			}
		}
	}

	storage_.clear();

	polygon cur;
	for (const auto& it : res_path)
		bg::append(cur, point(it.first, it.second));

	storage_ = {cur};
}

int RArea::Square() const {
	return bg::area(storage_);
}

RArea Intersection(RArea r1, RArea r2) {

	multi_polygon res;

	//std::cout << RArea(r1.GetStorage()).Square() << " " << RArea(r2.GetStorage()).Square() << std::endl; 
	//std::vector<multi_polygon> res;
	bg::intersection(r1.GetStorage(), r2.GetStorage(), res);
	bg::correct(res);

	//std::cout << res.size() << std::endl;
	return RArea(res);
}

RArea Merge(RArea r1, RArea r2) {
	multi_polygon res;

	bg::union_(r1.GetStorage(), r2.GetStorage(), res);
	bg::correct(res);

	return RArea(res);
}


int IntersectionSize(RArea r1, RArea r2) {
	
	RArea res = Intersection(r1, r2);

	return res.Square();
}

const multi_polygon& RArea::GetStorage() const {
	return storage_;
}


RArea::RArea(const multi_polygon& storage) :
	storage_(storage)
{
	Compress();
	bg::correct(storage_);
}
