#include <gtest/gtest.h>

#include <thread>
#include "perg/queue.h"

struct queue_test : public ::testing::Test
{
	perg::queue<int> q;
};

struct stress_test
{
	enum {LENGTH = 50000};
	std::vector<int> expected;
	std::vector<int> actual;
	std::thread reader;

	stress_test()
	{
		generate();
	}

	void spawn_reader(perg::queue<int>& queue)
	{
		reader = std::thread([&]{
			while (expected.size() < LENGTH) {
				expected.push_back(queue.pop());
			}});
	}

	void read_batch(perg::queue<int>& queue)
	{
		reader = std::thread([&]{
			while (expected.size() < LENGTH) {
				perg::list<int> ll = queue.pop_all();
				for (int i : ll)
				{
					expected.push_back(i);	
				}
			}});
	}

	void generate()
	{
		actual.resize(LENGTH); int seed = 0;
		std::generate(actual.begin(), actual.end(), [&](){return seed++;});
	}

	void write_queue(perg::queue<int>& queue)
	{
		for (int i : actual)
		{
			queue.push(i);
		}

		reader.join();
	}
	
	void verify()
	{
		EXPECT_TRUE(actual == expected);
	}
};

TEST_F(queue_test, enqueues_stuff)
{
	q.push(123);
	EXPECT_EQ(123, q.pop());
}

TEST_F(queue_test, queue_is_of_fifo_type)
{
	q.push(123);
	q.push(456);
	EXPECT_EQ(123, q.pop());
}

TEST_F(queue_test, reads_last_item_on_the_queue)
{
	q.push(123);
	q.push(456);
	q.pop();

	EXPECT_EQ(456, q.pop());
}

TEST_F(queue_test, initially_the_queue_is_empty)
{
	EXPECT_EQ(0, q.size());
}

TEST_F(queue_test, queue_size_grows_as_new_item_comes_in)
{
	q.push(123);
	EXPECT_EQ(1, q.size());
}

TEST_F(queue_test, queue_size_decreases_as_item_get_popped_out)
{
	q.push(123);
	q.pop();
	EXPECT_EQ(0, q.size());
}

TEST_F(queue_test, queue_size_remain_unchanged_if_unable_to_pop_an_item)
{
	int i = 0;
	q.try_pop(i);
	EXPECT_EQ(0, q.size());
}


TEST_F(queue_test, reads_everything_at_once)
{
	q.push(123);
	q.push(456);

	auto ll = q.pop_all();
	EXPECT_EQ(123, ll.front());
	EXPECT_EQ(456, ll.back());
}

TEST_F(queue_test, clears_the_queue)
{
	q.push(123);
	q.push(456);
	q.pop_all();

	EXPECT_EQ(0, q.size());
}

TEST_F(queue_test, unlimited_queue_stress_test)
{
	stress_test stress;
	stress.spawn_reader(q);
	stress.write_queue(q);

	stress.verify();
}

TEST_F(queue_test, limited_queue_stress_test)
{
	q.limit(1);

	stress_test stress;
	stress.spawn_reader(q);
	stress.write_queue(q);

	stress.verify();
}

TEST_F(queue_test, unlimited_queue_batch_reader_stress_test)
{
	stress_test stress;
	stress.read_batch(q);
	stress.write_queue(q);

	stress.verify();
}

TEST_F(queue_test, limited_queue_batch_reader_stress_test)
{
	q.limit(1);

	stress_test stress;
	stress.read_batch(q);
	stress.write_queue(q);

	stress.verify();
}
