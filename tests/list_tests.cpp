#include <gtest/gtest.h>
#include "perg/intrusive.h"

struct list_tests : public ::testing::Test
{
	using mylist = perg::list<int>;
	mylist list;
};

TEST_F(list_tests, list_is_empty_once_created)
{
	EXPECT_TRUE(list.empty());
}

TEST_F(list_tests, list_is_not_empty)
{
	list.push_back(123);
	EXPECT_TRUE(!list.empty());
}

TEST_F(list_tests, insert_node_in_empty_list)
{
	list.push_back(123);
	EXPECT_EQ(123, list.front()); 
}


TEST_F(list_tests, back_points_to_newly_inserted_node)
{
	list.push_back(123);
	EXPECT_EQ(123, list.back()); 
}

TEST_F(list_tests, inserts_several_nodes)
{
	list.push_back(123);
	list.push_back(456);
	list.push_back(789);

	EXPECT_EQ(123, list.front()); 
	EXPECT_EQ(789, list.back());
}

TEST_F(list_tests, count_list_size)
{
	list.push_back(123);
	list.push_back(456);

	EXPECT_EQ(2, list.size());
}

TEST_F(list_tests, empty_list_has_size_zero)
{
	EXPECT_EQ(0, list.size());
}

TEST_F(list_tests, empties_list_by_popping_a_node_from_back)
{
	list.push_back(123);

	list.pop_back();
	EXPECT_EQ(0, list.size());
}

TEST_F(list_tests, empties_list_by_poppsing_a_node_from_front)
{
	list.push_back(123);

	list.pop_front();
	EXPECT_EQ(0, list.size());
}

TEST_F(list_tests, pops_node_from_front)
{
	list.push_back(123);
	list.push_back(456);
	list.pop_front();

	EXPECT_EQ(456, list.front());
}

TEST_F(list_tests, pops_node_from_back)
{
	list.push_back(123);
	list.push_back(456);
	list.pop_back();

	EXPECT_EQ(123, list.back());
}

TEST_F(list_tests, clears_list)
{
	list.push_back(123);
	list.clear();

	EXPECT_TRUE(list.empty());
}

