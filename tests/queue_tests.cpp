#include <gtest/gtest.h>
#include "perg/queue.h"

struct perg_test : public ::testing::Test
{
	perg::queue<int> q;
};

TEST_F(perg_test, enqueues_stuff)
{
	q.push(123);
	EXPECT_EQ(123, q.pop());
}

TEST_F(perg_test, queue_is_of_fifo_type)
{
	q.push(123);
	q.push(456);
	EXPECT_EQ(123, q.pop());
}

TEST_F(perg_test, reads_last_item_on_the_queue)
{
	q.push(123);
	q.push(456);
	q.pop();

	EXPECT_EQ(456, q.pop());
}

