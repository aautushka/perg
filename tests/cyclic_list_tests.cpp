#include <gtest/gtest.h>
#include "perg/cyclic_list.h"

struct cyclic_list_test : public ::testing::Test
{
	perg::cyclic_list<int, 4> list;
};

TEST_F(cyclic_list_test, list_is_empty_once_created)
{
	EXPECT_TRUE(list.empty());
	EXPECT_EQ(0, list.size());
	EXPECT_FALSE(list.full());
}

TEST_F(cyclic_list_test, list_is_no_longer_empty)
{
	list.push_back(123);
	EXPECT_FALSE(list.empty());
}

TEST_F(cyclic_list_test, accesses_element_just_added_to_the_tail)
{
	list.push_back(123);
	EXPECT_EQ(123, list.front());
	EXPECT_EQ(123, list.back());
}

TEST_F(cyclic_list_test, accesses_element_just_added_to_the_head)
{
	list.push_front(123);
	EXPECT_EQ(123, list.front());
	EXPECT_EQ(123, list.back());
}

TEST_F(cyclic_list_test, accesses_head_and_tail_separately)
{
	list.push_back(123);
	list.push_back(456);

	EXPECT_EQ(123, list.front());
	EXPECT_EQ(456, list.back());
}

TEST_F(cyclic_list_test, pushes_elements_to_the_head)
{
	list.push_front(123);
	list.push_front(456);

	EXPECT_EQ(456, list.front());
	EXPECT_EQ(123, list.back());
}

TEST_F(cyclic_list_test, list_capacity_matches_the_template_argument)
{
	EXPECT_EQ(4, list.capacity());
}

TEST_F(cyclic_list_test, list_is_not_full_until_consumes_all_capacity)
{
	list.push_back(1);
	list.push_back(2);
	list.push_back(3);

	EXPECT_FALSE(list.full());
}

TEST_F(cyclic_list_test, list_is_full)
{
	list.push_back(1);
	list.push_back(2);
	list.push_back(3);
	list.push_back(4);

	EXPECT_EQ(4, list.size());
	EXPECT_TRUE(list.full());
}

TEST_F(cyclic_list_test, reaches_full_capacity_by_growing_list_from_its_head)
{
	list.push_front(1);
	list.push_front(2);
	list.push_front(3);
	list.push_front(4);

	EXPECT_EQ(4, list.size());
	EXPECT_TRUE(list.full());
}


TEST_F(cyclic_list_test, clears_list)
{
	list.push_back(1);
	list.clear();

	EXPECT_TRUE(list.empty());
}

TEST_F(cyclic_list_test, push_back_changes_list_size)
{
	list.push_back(1);

	EXPECT_EQ(1, list.size());
}

TEST_F(cyclic_list_test, push_front_changes_list_size)
{
	list.push_front(1);

	EXPECT_EQ(1, list.size());
}

TEST_F(cyclic_list_test, pop_front_changes_list_size)
{
	list.push_front(1);
	list.pop_front();

	EXPECT_EQ(0, list.size());
}

TEST_F(cyclic_list_test, pop_back_changes_list_size)
{
	list.push_front(1);
	list.pop_back();

	EXPECT_EQ(0, list.size());
}

TEST_F(cyclic_list_test, pop_back_returns_element_being_removed)
{
	list.push_back(1);
	list.push_back(2);

	EXPECT_EQ(2, list.pop_back());
}

TEST_F(cyclic_list_test, pop_front_returns_element_being_removed)
{
	list.push_back(1);
	list.push_back(2);

	EXPECT_EQ(1, list.pop_front());
}

TEST_F(cyclic_list_test, list_at_full_capacity_it_not_empty)
{
	list.push_back(1);
	list.push_back(2);
	list.push_back(3);
	list.push_back(4);

	EXPECT_FALSE(list.empty());
}

TEST_F(cyclic_list_test, iterates_over_list)
{
	list.push_back(1);
	list.push_back(2);
	list.push_back(3);
	list.push_back(4);

	std::vector<int> expected = {1, 2, 3, 4};
	std::vector<int> actual;

	for (int i : list ) actual.push_back(i);
	EXPECT_EQ(expected, actual);

}

TEST_F(cyclic_list_test, creates_element_at_the_back)
{
	list.push_back(123);
	list.emplace_back() = 456;

	EXPECT_EQ(456, list.back()); 
}

TEST_F(cyclic_list_test, creates_element_at_the_front)
{
	list.push_back(123);
	list.emplace_front() = 456;

	EXPECT_EQ(456, list.front()); 
}
