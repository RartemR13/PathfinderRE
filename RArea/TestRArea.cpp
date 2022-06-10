#include <gtest/gtest.h>

#include "RArea.hpp"
#include "TestRArea.hpp"

#include <stdexcept>
#include <exception>
#include <sstream>
#include <iostream>
#include <map>

void TestRArea::SetUp() {
	std::vector<std::pair<int, int>> data = 
		{{0, 4},
		 {0, 4}};
	std::vector<int> lines = 
		{0, 1};
	base1 = RArea(data, lines);

	data =
		{{0, 4},
	     {1, 4},
	 	 {0, 4},
	 	 {0, 4}};
	lines = 
		{0, 1, 2, 3};
	base2 = RArea(data, lines);

	data = 
		{{4, 5},
		 {4, 5},
		 {4, 5},
		 {2, 6}};
	lines = 
		{0, 1, 2, 3};
	base3 = RArea(data, lines);

	data = 
		{};
	lines =
		{};
	base4 = RArea(data, lines);
}

bool CheckVertexPath(const std::vector<std::pair<int, int>>& p1, 
					 const std::vector<std::pair<int, int>>& p2)
{
	std::map<std::pair<int, int>, int> point_num;
	int cur_num = 0;

	for (const auto& point : p1)
		if (point_num.find(point) == point_num.end())
			point_num[point] = cur_num++;

	for (const auto& point : p2)
		if (point_num.find(point) == point_num.end())
			return false;

	std::vector<std::vector<int>> adj_list_p1(point_num.size()),
								  adj_list_p2(point_num.size());

	for (int i = 1; i < p1.size(); ++i)
		adj_list_p1[point_num[p1[i-1]]].push_back(point_num[p1[i]]);

	for (int i = 1; i < p2.size(); ++i)
		adj_list_p2[point_num[p2[i-1]]].push_back(point_num[p2[i]]);

	return adj_list_p1 == adj_list_p2;
}

TEST_F(TestRArea, test_rarea_base_1)
{
	std::vector<std::pair<int, int>> res_path = base1.GetVertexPath();
	std::vector<std::pair<int, int>> exp_path =
		{{0, 0},
	     {0, 2},
	     {5, 2},
	 	 {5, 0},
	 	 {0, 0}};

	ASSERT_TRUE(CheckVertexPath(res_path, exp_path));
}

TEST_F(TestRArea, test_rarea_base_2)
{
	std::vector<std::pair<int, int>> res_path = base2.GetVertexPath();
	std::vector<std::pair<int, int>> exp_path =
		{{0, 0},
		 {0, 1},
		 {1, 1},
		 {1, 2},
		 {0, 2},
		 {0, 4},
		 {5, 4},
		 {5, 0},
		 {0, 0}};

	ASSERT_TRUE(CheckVertexPath(res_path, exp_path));
}

TEST_F(TestRArea, test_rarea_base_3)
{
	std::vector<std::pair<int, int>> res_path = base3.GetVertexPath();
	std::vector<std::pair<int, int>> exp_path =
		{{4, 0},
		 {4, 3},
		 {2, 3},
		 {2, 4},
		 {7, 4},
		 {7, 3},
		 {6, 3},
		 {6, 0},
		 {4, 0}};

	ASSERT_TRUE(CheckVertexPath(res_path, exp_path));
}

TEST_F(TestRArea, test_rarea_base_4)
{
	std::vector<std::pair<int, int>> res_path = base4.GetVertexPath();
	std::vector<std::pair<int, int>> exp_path =
		{};

	for (int i = 0; i < res_path.size(); ++i)
		std::cout << res_path[i].first << " " << res_path[i].second << std::endl;

	ASSERT_TRUE(CheckVertexPath(res_path, exp_path));
}

TEST_F(TestRArea, test_rarea_base_1_square)
{
	ASSERT_EQ(base1.Square(), 10);
}

TEST_F(TestRArea, test_rarea_base_2_square)
{
	ASSERT_EQ(base2.Square(), 19);
}

TEST_F(TestRArea, test_rarea_base_3_square)
{
	ASSERT_EQ(base3.Square(), 11);
}

TEST_F(TestRArea, test_rarea_intersection_base_1_base_2)
{
	RArea res_inters = Intersection(base1, base2);

	std::vector<std::pair<int, int>> res_path = res_inters.GetVertexPath();
	std::vector<std::pair<int, int>> exp_path = 
		{{0, 0},
		 {0, 1},
		 {1, 1},
		 {1, 2},
		 {5, 2},
		 {5, 0},
		 {0, 0}};

	ASSERT_TRUE(CheckVertexPath(res_path, exp_path));	
}

TEST_F(TestRArea, test_rarea_intersection_base_2_base_1)
{
	RArea res_inters = Intersection(base2, base1);

	std::vector<std::pair<int, int>> res_path = res_inters.GetVertexPath();
	std::vector<std::pair<int, int>> exp_path = 
		{{0, 0},
		 {0, 1},
		 {1, 1},
		 {1, 2},
		 {5, 2},
		 {5, 0},
		 {0, 0}};

	ASSERT_TRUE(CheckVertexPath(res_path, exp_path));
}

TEST_F(TestRArea, test_rarea_merge_base_1_base_2)
{
	RArea res_merge = Merge(base1, base2);
	std::vector<std::pair<int, int>> res_path = res_merge.GetVertexPath();
	std::vector<std::pair<int, int>> exp_path = 
		{{0, 0},
		 {0, 4},
		 {5, 4},
		 {5, 0},
		 {0, 0}};

	ASSERT_TRUE(CheckVertexPath(res_path, exp_path));
}

TEST_F(TestRArea, test_rarea_merge_base_2_base_1)
{
	RArea res_merge = Merge(base2, base1);
	std::vector<std::pair<int, int>> res_path = res_merge.GetVertexPath();
	std::vector<std::pair<int, int>> exp_path = 
		{{0, 0},
		 {0, 4},
		 {5, 4},
		 {5, 0},
		 {0, 0}};

	ASSERT_TRUE(CheckVertexPath(res_path, exp_path));
}

TEST_F(TestRArea, test_rarea_intersection_base_1_base_2_square)
{
	ASSERT_EQ(Intersection(base1, base2).Square(), 9);
}

TEST_F(TestRArea, test_rarea_intersection_base_2_base_1_square)
{
	ASSERT_EQ(Intersection(base2, base1).Square(), 9);
}

TEST_F(TestRArea, test_rarea_intersection_base_1_base_4_square)
{
	ASSERT_EQ(Intersection(base1, base4).Square(), 0);
}

TEST_F(TestRArea, test_rarea_merge_base_1_base_2_square)
{	
	ASSERT_EQ(Merge(base1, base2).Square(), 20);
}

TEST_F(TestRArea, test_rarea_merge_base_2_base_1_square)
{
	ASSERT_EQ(Merge(base2, base1).Square(), 20);
}




int main(int argc, char* argv[]) {
	::testing::InitGoogleTest(&argc, argv);

	return RUN_ALL_TESTS();
}