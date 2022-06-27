#pragma once

#include "SHA256/include/SHA256.h"
#include <vector>
#include <fstream>

class AdjListSaver {
public:
	void DumpFormsHash();
	void DumpAdjList();

	void SetAdjList(const std::vector<std::vector<int>>& AdjList);
	void SetHash(const std::vector<std::pair<int, int>>& forms);

	void Pull();
	const std::vector<std::vector<int>>& Get();

	bool CheckHash(const std::vector<std::pair<int, int>>& forms);

private:
	std::vector<std::pair<int, int>> forms_;
	std::vector<std::vector<int>> AdjList_;
	std::string hash_;
};