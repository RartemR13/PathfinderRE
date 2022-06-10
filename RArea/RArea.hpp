#pragma once

#include <string>
#include <vector>

#include <boost/geometry/geometry.hpp>
#include <boost/geometry/geometries/point_xy.hpp>
#include <boost/geometry/geometries/polygon.hpp>

namespace bgm = boost::geometry::model;

class RArea {
public:
	RArea() {}
	RArea(const std::vector<std::string>& data);
	RArea(const bgm::multi_polygon<bgm::polygon<bgm::d2::point_xy<int>>>& storage);

	RArea(const std::vector<std::pair<int, int>>& data, const std::vector<int>& lines);
	std::vector<std::pair<int, int>> GetVertexPath();
	void Compress();

	int Square() const; 
	const bgm::multi_polygon<bgm::polygon<bgm::d2::point_xy<int>>>& GetStorage() const;

	friend RArea Intersection(RArea r1, RArea r2);
	friend RArea Merge(RArea r1, RArea r2);
	friend int IntersectionSize(RArea r1, RArea r2);

private:
	bgm::multi_polygon<bgm::polygon<bgm::d2::point_xy<int>>> storage_;
};