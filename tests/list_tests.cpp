#include <gtest/gtest.h>
#include "perg/intrusive.h"
#include "perg/block_list.h"

template <typename T>
struct list_tests : public ::testing::Test
{
	using mylist = T;
	mylist list;
};

typedef ::testing::Types<
		perg::list<int>,
		perg::block_list<int, 1>,
		perg::block_list<int, 2>,
		perg::block_list<int, 4>,
		perg::block_list<int, 8>,
		perg::block_list<int, 16>,
		perg::block_list<int, 1024>,
		perg::block_list<int, 1024 * 1024>,
		perg::block_list<int, 4 * 1024 * 1024>
	> TestTypes;
TYPED_TEST_CASE(list_tests, TestTypes);


TYPED_TEST(list_tests, list_is_empty_once_created)
{
	EXPECT_TRUE(this->list.empty());
}

TYPED_TEST(list_tests, list_is_not_empty)
{
	this->list.push_back(123);
	EXPECT_TRUE(!this->list.empty());
}

TYPED_TEST(list_tests, insert_node_in_empty_list)
{
	this->list.push_back(123);
	EXPECT_EQ(123, this->list.front()); 
}


TYPED_TEST(list_tests, back_points_to_newly_inserted_node)
{
	this->list.push_back(123);
	EXPECT_EQ(123, this->list.back()); 
}

TYPED_TEST(list_tests, inserts_several_nodes)
{
	this->list.push_back(123);
	this->list.push_back(456);
	this->list.push_back(789);

	EXPECT_EQ(123, this->list.front()); 
	EXPECT_EQ(789, this->list.back());
}

TYPED_TEST(list_tests, count_list_size)
{
	this->list.push_back(123);
	this->list.push_back(456);

	EXPECT_EQ(2, this->list.size());
}

TYPED_TEST(list_tests, empty_list_has_size_zero)
{
	EXPECT_EQ(0, this->list.size());
}

TYPED_TEST(list_tests, empties_list_by_popping_a_node_from_back)
{
	this->list.push_back(123);

	this->list.pop_back();
	EXPECT_EQ(0, this->list.size());
}

TYPED_TEST(list_tests, empties_list_by_poppsing_a_node_from_front)
{
	this->list.push_back(123);

	this->list.pop_front();
	EXPECT_EQ(0, this->list.size());
}

TYPED_TEST(list_tests, pops_node_from_front)
{
	this->list.push_back(123);
	this->list.push_back(456);
	this->list.pop_front();

	EXPECT_EQ(456, this->list.front());
}

TYPED_TEST(list_tests, pops_node_from_back)
{
	this->list.push_back(123);
	this->list.push_back(456);
	this->list.pop_back();

	EXPECT_EQ(123, this->list.back());
}

TYPED_TEST(list_tests, clears_list)
{
	this->list.push_back(123);
	this->list.clear();

	EXPECT_TRUE(this->list.empty());
}

TYPED_TEST(list_tests, concats_lists)
{
	this->list.push_back(123);

	TypeParam other;
	other.push_back(456);

	this->list.push_back(std::move(other));
	EXPECT_EQ(2, this->list.size());
}

TYPED_TEST(list_tests, concats_long_lists)
{
	this->list.push_back(12);
	this->list.push_back(34);
	ASSERT_EQ(2, this->list.size());

	TypeParam other;
	other.push_back(56);
	other.push_back(78);
	ASSERT_EQ(2, this->list.size());

	this->list.push_back(std::move(other));
	EXPECT_EQ(4, this->list.size());
}

TYPED_TEST(list_tests, initializes_list)
{
	TypeParam other;
	other.push_back(456);

	this->list.push_back(std::move(other));
	EXPECT_EQ(1, this->list.size());
}

TYPED_TEST(list_tests, appends_empty_list)
{
	TypeParam other;

	this->list.push_back(123);
	this->list.push_back(std::move(other));
	EXPECT_EQ(1, this->list.size());
}

TYPED_TEST(list_tests, creates_element_at_the_back)
{
	this->list.push_back(123);
	this->list.emplace_back() = 456;

	EXPECT_EQ(456, this->list.back()); 
}

TYPED_TEST(list_tests, creates_element_at_the_front)
{
	this->list.push_back(123);
	this->list.emplace_front() = 456;

	EXPECT_EQ(456, this->list.front()); 
}
