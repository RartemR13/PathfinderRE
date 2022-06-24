#include "XPSHelper.hpp"

#include <fstream>
#include <stdexcept>
#include <string>
#include <algorithm>
#include <iostream>

int GetVertexX(const std::string& xps_line) {
	std::string info_block = xps_line.substr(29, 4);
	reverse(info_block.begin(), info_block.end());
	while (info_block.back() == ' ' or info_block.back() == '0')
		info_block.pop_back();
	reverse(info_block.begin(), info_block.end());

	return std::stoi(info_block);
}

int GetVertexY(const std::string& xps_line) {
	std::string info_block = xps_line.substr(33, 4);
	std::reverse(info_block.begin(), info_block.end());
	while (info_block.back() == ' ' or info_block.back() == '0')
		info_block.pop_back();
	std::reverse(info_block.begin(), info_block.end());

	return std::stoi(info_block);
}

void DeleteUnusedXPSLines(std::vector<std::string>& xps_lines) {
	std::vector<std::string> new_xps_lines;

	int cur_segment_size = 0;

	for (int i = 0; i < xps_lines.size(); ++i) {
		if (not new_xps_lines.empty() and 
			std::make_pair(GetVertexX(new_xps_lines.back()), GetVertexY(new_xps_lines.back())) !=
				std::make_pair(GetVertexX(xps_lines[i]), GetVertexY(xps_lines[i])))
		{
			i += cur_segment_size * 5;
			cur_segment_size = 0;

			if (i >= xps_lines.size())
				break;
		}

		new_xps_lines.push_back(xps_lines[i]);
		cur_segment_size++;
	}

	xps_lines = new_xps_lines;
}

XPSHelper::XPSHelper(const char* XPSFilePath) {
	std::cout << "Start XPSHelper for " << XPSFilePath << std::endl;

	std::ifstream XPSIn(XPSFilePath, std::ios::in);
	if (not XPSIn.is_open())
		throw std::runtime_error("Can not open XPSFilePath");

	std::vector<std::string> xpsLines;
	std::string line;
	while(std::getline(XPSIn, line))
		xpsLines.push_back(line);

	XPSIn.close();

	DeleteUnusedXPSLines(xpsLines);

	std::cout << "Deleting unused XPS Lines OK" << std::endl;

	int SubX = GetVertexX(xpsLines[0]),
		SubY = GetVertexY(xpsLines[0]);

	for (const auto& curXpsLine : xpsLines) {
		int X = GetVertexX(curXpsLine),
			Y = GetVertexY(curXpsLine);

		if (geometry_.empty() || geometry_.back() != std::make_pair(X - SubX, Y - SubY)) {
			geometry_.push_back({X - SubX, Y - SubY});
			oldGeometry_.push_back({X, Y});
		}
	}

	std::cout << "Old Geometry OK" << std::endl;

	std::vector<std::pair<int, int>> linesSegments;
	int curVertexStart = 0,
		curLineNum     = 0,
		curVertexNum   = 0;

	for (const auto& curXpsLine : xpsLines) {
		if (std::make_pair(GetVertexX(curXpsLine) - SubX, GetVertexY(curXpsLine) - SubY) != geometry_[curVertexNum]) {
			linesSegments.push_back({curVertexStart, curLineNum-1});
			curVertexStart = curLineNum;
			curVertexNum++;
		}
		curLineNum++;
	}
	linesSegments.push_back({curVertexStart, curLineNum-1});

	for (const auto& segment : linesSegments) {
		std::vector<std::string> segmentLines;
		for (int i = segment.first; i <= segment.second; ++i)
			segmentLines.push_back(xpsLines[i]);

		storage_.push_back(RArea(segmentLines));
	}

	std::cout << "Storage OK" << std::endl;

	int gcd_x = std::__gcd(geometry_[0].first, geometry_[1].first);
	for (int i = 2; i < geometry_.size(); ++i)
		gcd_x = std::__gcd(gcd_x, geometry_[i].first);
	if (gcd_x > 0)
		for (int i = 0; i < geometry_.size(); ++i)
			geometry_[i].first /= gcd_x;

	int gcd_y = std::__gcd(geometry_[0].second, geometry_[1].second);
	for (int i = 2; i < geometry_.size(); ++i)
		gcd_y = std::__gcd(gcd_y, geometry_[i].second);
	if (gcd_y > 0)
		for (int i = 0; i < geometry_.size(); ++i)
			geometry_[i].second /= gcd_y;

	for (int i = 0; i < geometry_.size(); ++i)
		geomToNum_[geometry_[i]] = i;

	std::cout << "Geometry OK" << std::endl;
	std::cout << "XPSHelper constructor OK" << std::endl;
}

int XPSHelper::GetWellCount() const {
	return storage_.size();
}

int XPSHelper::GetWellNum(std::pair<int, int> coords) const {
	return geomToNum_.at(coords);
}

std::pair<int, int> XPSHelper::GetWellCoords(int num) const {
	return geometry_[num];
}

RArea XPSHelper::GetWellCoat(int num) const {
	return storage_[num];
}

std::pair<int, int> XPSHelper::GetOldCoords(int num) const {
	return oldGeometry_[num];
}