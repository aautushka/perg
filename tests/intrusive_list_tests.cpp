#include <gtest/gtest.h>
#include "perg/intrusive.h"

struct intrusive_list_tests : public ::testing::Test
{
	using mylist = perg::list<int>;
	mylist list;
};

TEST_F(intrusive_list_tests, list_is_empty_once_created)
{
	EXPECT_TRUE(list.empty());
}

TEST_F(intrusive_list_tests, list_is_not_empty)
{
	mylist::node node(123);	
	list.push_back(node);
	EXPECT_TRUE(!list.empty());
}

TEST_F(intrusive_list_tests, insert_node_in_empty_list)
{
	mylist::node node(123);	
	list.push_back(node);
	EXPECT_EQ(123, list.front().data); 
}


TEST_F(intrusive_list_tests, back_points_to_newly_inserted_node)
{
	mylist::node node(123);	
	list.push_back(node);
	EXPECT_EQ(123, list.back().data); 
}

TEST_F(intrusive_list_tests, inserts_several_nodes)
{
	mylist::node n1(123);	
	mylist::node n2(456);
	mylist::node n3(789);

	list.push_back(n1);
	list.push_back(n2);
	list.push_back(n3);
	EXPECT_EQ(123, list.front().data); 
	EXPECT_EQ(789, list.back().data);
}

TEST_F(intrusive_list_tests, count_list_size)
{
	mylist::node n1(123);	
	mylist::node n2(456);

	list.push_back(n1);
	list.push_back(n2);

	EXPECT_EQ(2, list.size());
}

TEST_F(intrusive_list_tests, empty_list_has_size_zero)
{
	EXPECT_EQ(0, list.size());
}

TEST_F(intrusive_list_tests, empties_list_by_popping_a_node_from_back)
{
	mylist::node n(123);
	list.push_back(n);

	list.pop_back();
	EXPECT_EQ(0, list.size());
}

TEST_F(intrusive_list_tests, empties_list_by_poppsing_a_node_from_front)
{
	mylist::node n(123);
	list.push_back(n);

	list.pop_front();
	EXPECT_EQ(0, list.size());
}
