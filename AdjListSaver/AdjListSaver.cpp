#include "AdjListSaver.hpp"

#include <sstream>
#include <stdexcept>

void AdjListSaver::DumpFormsHash() {
	std::ofstream out("FORMS_HASH", std::ios::out);
	out << hash_;
	out.close();
}

void AdjListSaver::DumpAdjList() {
	std::ofstream out("ADJ_LIST", std::ios::out);
	out << AdjList_.size() << "\n";
	for (const auto& vec : AdjList_) {
		out << vec.size() << " ";
		for (auto it : vec)
			out << it << " ";
		out << "\n";
	}
	out.close();
}

void AdjListSaver::SetAdjList(const std::vector<std::vector<int>>& AdjList) {
	AdjList_ = AdjList;
}

void AdjListSaver::SetHash(const std::vector<std::pair<int, int>>& forms) {
	std::stringstream str;
	for (int i = 0; i < forms.size(); ++i) {
		str << forms[i].first << "#" << forms[i].second << "!";
	}

	SHA256 toHash;
	toHash.update(str.str());

	auto digest = toHash.digest();

	hash_ = toHash.toString(digest);
	delete[] digest;
}

void AdjListSaver::Pull() {
	std::ifstream in("ADJ_LIST", std::ios::in);
	AdjList_.clear();

	if (not in.is_open())
		throw std::runtime_error("");

	int size = 0;
	in >> size;

	AdjList_.resize(size);

	for (int i = 0; i < size; ++i) {
		int vec_size = 0;
		in >> vec_size;

		for (int j = 0; j < vec_size; ++j) {
			int x = 0;
			in >> x;

			AdjList_[i].push_back(x);
		}
	}
	in.close();

	in.open("FORMS_HASH", std::ios::in);
	if (not in.is_open())
		throw std::runtime_error("");

	std::string hash;
	in >> hash;
	in.close();

	hash_ = hash;
}

const std::vector<std::vector<int>>& AdjListSaver::Get() {
	return AdjList_;
}

bool AdjListSaver::CheckHash(const std::vector<std::pair<int, int>>& forms) {
	std::stringstream str;
	for (int i = 0; i < forms.size(); ++i) {
		str << forms[i].first << "#" << forms[i].second << "!";
	}

	SHA256 toCheck;
	toCheck.update(str.str());

	auto digest = toCheck.digest();

	bool ret = (hash_ == toCheck.toString(digest));

	delete[] digest;
	return ret;
}

