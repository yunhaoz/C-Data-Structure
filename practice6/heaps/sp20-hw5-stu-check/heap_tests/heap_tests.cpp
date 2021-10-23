//
// CS104 Heap test suite
//

#include <heap.h>

#include <gtest/gtest.h>

#include <random_generator.h>

#include <queue>
#include <algorithm>

#define RANDOM_HEAP_ARY 0
const int MAX_HEAP_ARY = 26;
const int DEFAULT_HEAP_ARY = 2;

// Declaration for the type of item that will be stored in the heap.
typedef int IntType;

// The random seed used to initialize the random number generator. If set to std::random_device()() then the
// tests will truly be random. Otherwise it should be set to some literal integer.
#define RANDOM_SEED 57821

// Testing setup.
class HeapTest : public testing::Test
{
protected:
	HeapTest()
	{
#if RANDOM_HEAP_ARY
		__RANDOM_NUMBER_GENERATOR randomGenerator{ RANDOM_SEED };
			D_ARY = randomGenerator() % MAX_HEAP_ARY + 3;
#else
		D_ARY = DEFAULT_HEAP_ARY;
#endif
	}
	~HeapTest() { }
	virtual void SetUp() { }
	virtual void TearDown() { }

	int D_ARY;
};

/**
 * Check that the given heap contains the elements from contents.
 * Will remove all elements from the heap.
 * Assumes that each integer has been inserted into the heap with priority the same as the integer.
 * @param heap
 * @param contents
 * @return
 */
testing::AssertionResult checkHeapContents(Heap<int> & heap, std::vector<int> contents)
{
	// sort vector least to greatest so that the order of elements matches the order in the heap
	std::sort(contents.begin(), contents.end());

	for(size_t index = 0; index < contents.size(); ++index)
	{
		if(heap.empty())
		{
			return testing::AssertionFailure() << "Heap is missing " << (contents.size() - index) << " elements!  Only has " << index << ", should have had " << contents.size();
		}

		if(heap.top() != contents[index])
		{
			return testing::AssertionFailure() << "Incorrect element value after removing " << index << " elements!  Was " << heap.top() << ", expected " << contents[index];
		}

		heap.pop();
	}

	if(!heap.empty())
	{
		return testing::AssertionFailure() << "Heap has extra elements!  Should have had " << contents.size() << "elements";
	}

	return testing::AssertionSuccess();
}

TEST_F(HeapTest, ConstructionDestruction)
{
	Heap<IntType> heap(D_ARY);
}

TEST_F(HeapTest, InitiallyEmpty)
{
	Heap<IntType> heap(D_ARY);
	EXPECT_TRUE(heap.empty());
}

TEST_F(HeapTest, EmptyTopException)
{
	Heap<IntType> heap(D_ARY);
	EXPECT_ANY_THROW(heap.top());
}

TEST_F(HeapTest, EmptyPopException)
{
	Heap<IntType> heap(D_ARY);
	EXPECT_ANY_THROW(heap.pop());
}

TEST_F(HeapTest, SingleItemNonempty)
{
	Heap<IntType> heap(D_ARY);
	heap.push(1);
	EXPECT_FALSE(heap.empty());
}

TEST_F(HeapTest, SingleItemPush)
{
	Heap<IntType> heap(D_ARY);
	heap.push(1);
	EXPECT_EQ(1, heap.top());
}

TEST_F(HeapTest, SingleItemPushPop)
{
	Heap<IntType> heap(D_ARY);
	heap.push(1);
	heap.pop();
	EXPECT_TRUE(heap.empty());
}

TEST_F(HeapTest, TwoElementPush_LowPriFirst)
{
	Heap<IntType> heap(D_ARY);
	heap.push(3);
	heap.push(10);
	EXPECT_EQ(3, heap.top());

	heap.pop();

	EXPECT_EQ(10, heap.top());
}

TEST_F(HeapTest, TwoElementPush_HighPriFirst)
{
	Heap<IntType> heap(D_ARY);
	heap.push(50);
	heap.push(49);
	EXPECT_EQ(49, heap.top());

	heap.pop();

	EXPECT_EQ(50, heap.top());
}

TEST_F(HeapTest, Duplicates)
{
	Heap<IntType> heap(D_ARY);
	heap.push(9);
	heap.push(9);
	heap.push(20);

	EXPECT_EQ(9, heap.top());

	heap.pop();
	heap.pop();

	EXPECT_EQ(20, heap.top());
}

TEST_F(HeapTest, NegativeElements)
{
	Heap<IntType> heap(D_ARY);
	heap.push(9999);
	heap.push(-9999);
	EXPECT_EQ(-9999, heap.top());
}

TEST_F(HeapTest, ThreeElementPush_0_5_10)
{
	Heap<IntType> heap(D_ARY);
	heap.push(0);
	heap.push(5);
	heap.push(10);
	EXPECT_EQ(0, heap.top());
	heap.pop();
	EXPECT_EQ(5, heap.top());
	heap.pop();
	EXPECT_EQ(10, heap.top());
}

TEST_F(HeapTest, ThreeElementPush_0_10_5)
{
	Heap<IntType> heap(D_ARY);
	heap.push(0);
	heap.push(10);
	heap.push(5);
	EXPECT_EQ(0, heap.top());
	heap.pop();
	EXPECT_EQ(5, heap.top());
	heap.pop();
	EXPECT_EQ(10, heap.top());
}

TEST_F(HeapTest, ThreeElementPush_5_0_10)
{
	Heap<IntType> heap(D_ARY);
	heap.push(5);
	heap.push(0);
	heap.push(10);
	EXPECT_EQ(0, heap.top());
	heap.pop();
	EXPECT_EQ(5, heap.top());
	heap.pop();
	EXPECT_EQ(10, heap.top());
}

TEST_F(HeapTest, ThreeElementPush_5_10_0)
{
	Heap<IntType> heap(D_ARY);
	heap.push(5);
	heap.push(10);
	heap.push(0);
	EXPECT_EQ(0, heap.top());
	heap.pop();
	EXPECT_EQ(5, heap.top());
	heap.pop();
	EXPECT_EQ(10, heap.top());
}

TEST_F(HeapTest, ThreeElementPush_10_0_5)
{
	Heap<IntType> heap(D_ARY);
	heap.push(10);
	heap.push(0);
	heap.push(5);
	EXPECT_EQ(0, heap.top());
	heap.pop();
	EXPECT_EQ(5, heap.top());
	heap.pop();
	EXPECT_EQ(10, heap.top());
}

TEST_F(HeapTest, ThreeElementPush_10_5_0)
{
	Heap<IntType> heap(D_ARY);
	heap.push(10);
	heap.push(5);
	heap.push(0);
	EXPECT_EQ(0, heap.top());
	heap.pop();
	EXPECT_EQ(5, heap.top());
	heap.pop();
	EXPECT_EQ(10, heap.top());
}

TEST_F(HeapTest, GreaterComparator)
{
	std::greater<IntType> comparator;

	Heap<IntType, std::greater<IntType>> heap(D_ARY, comparator);

	heap.push(10);
	heap.push(1000);
	heap.push(100);

	EXPECT_EQ(1000, heap.top());
	heap.pop();
	EXPECT_EQ(100, heap.top());
	heap.pop();
	EXPECT_EQ(10, heap.top());
}

TEST_F(HeapTest, StringStorage)
{
	Heap<std::string> stringHeap(D_ARY);

	stringHeap.push("blah");
	stringHeap.push("bluh");
	stringHeap.push("bloh");
	EXPECT_EQ("blah", stringHeap.top());
	stringHeap.pop();
	EXPECT_EQ("bloh", stringHeap.top());
	stringHeap.pop();
	EXPECT_EQ("bluh", stringHeap.top());
}

TEST_F(HeapTest, FiveElementPush_TrinaryHeap)
{
	Heap<IntType> heap(3);
	heap.push(0);
	heap.push(5);
	heap.push(10);
	heap.push(15);
	heap.push(20);

	EXPECT_EQ(0, heap.top());
	heap.pop();
	EXPECT_EQ(5, heap.top());
	heap.pop();
	EXPECT_EQ(10, heap.top());
	heap.pop();
	EXPECT_EQ(15, heap.top());
	heap.pop();
	EXPECT_EQ(20, heap.top());
}


TEST(HeapStress, 50x50RandomElements_QuaternaryHeap)
{
	const RandomSeed masterSeed = 99;
	const size_t numTrials = 50;
	const size_t numElements = 50;
	const size_t d = 4;

	std::vector<RandomSeed> seedVector = makeRandomSeedVector(numTrials, masterSeed);

	for(RandomSeed trialSeed : seedVector)
	{
		std::vector<int> data = makeRandomNumberVector<int>(numElements, 0, numElements * 10, trialSeed, true);
		Heap<int> heap(d);

		for(int value : data)
		{
			heap.push(value);
		}

		EXPECT_TRUE(checkHeapContents(heap, data));
	}
}
