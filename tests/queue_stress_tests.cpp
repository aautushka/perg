
#include <gtest/gtest.h>

#include <thread>
#include "perg/queue.h"

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
				auto ll = queue.pop_all();
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

struct queue_stress_test : public ::testing::Test
{
	perg::queue<int> q;
};


TEST_F(queue_stress_test, unlimited_queue_stress_test)
{
	stress_test stress;
	stress.spawn_reader(q);
	stress.write_queue(q);

	stress.verify();
}

TEST_F(queue_stress_test, limited_queue_stress_test)
{
	q.limit(1);

	stress_test stress;
	stress.spawn_reader(q);
	stress.write_queue(q);

	stress.verify();
}

TEST_F(queue_stress_test, unlimited_queue_batch_reader_stress_test)
{
	stress_test stress;
	stress.read_batch(q);
	stress.write_queue(q);

	stress.verify();
}

TEST_F(queue_stress_test, limited_queue_batch_reader_stress_test)
{
	q.limit(1);

	stress_test stress;
	stress.read_batch(q);
	stress.write_queue(q);

	stress.verify();
}
