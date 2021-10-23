//
// This file contains some helper code for our test suite.
// You shouldn't need to mess with it.
//

#ifndef CS104FINAL_TRIE_TRIE_TEST_FUNCTIONS_H
#define CS104FINAL_TRIE_TRIE_TEST_FUNCTIONS_H

#include "trie.h"

#include <string>
#include <vector>
#include <sstream>
#include <set>
#include <random>
#include <utility>
#include <map>
#include <iomanip>

#include <gtest/gtest.h>

// Check that a given node of the tree contains the given child nodes.
// This is needed because children are allowed to be in any order.
// Map contains elements and whether their terminal flag should be set.
testing::AssertionResult checkTrieNodeChildren(std::map<char, bool> expectedElements, TNode * parentNode);

typedef uint32_t RandomSeed;

/**
 * Make a vector of random words of random length from the lowercase characters a-z.  This is great for generating trie test data!
 * @param count How many words to generate.
 * @param seed Random seed.
 * @param maxLength Length of the words will be uniformly distributed between 0 and this number.
 * @param allowDuplicates Whether duplicates are allowed in the results.
 * @return
 */
std::vector<std::string> makeRandomWordVector(size_t count, RandomSeed seed, size_t maxLength, bool allowDuplicates);


// Converts a vector of items to a printable string
template<typename T>
std::string vectorToString(std::vector<T> const & items)
{
	std::stringstream output;
	output << '[';

	bool firstLoop = true;

	for(T const & item : items)
	{
		if(firstLoop)
		{
			firstLoop = false;
		}
		else
		{
			output << ", ";
		}

		output << item;
	}

	output << ']';

	return output.str();
}

// Converts a set of items to a printable string
template<typename T, typename Comparator>
std::string setToString(std::set<T, Comparator> const & items)
{
	std::stringstream output;
	output << '[';

	bool firstLoop = true;

	for(T const & item : items)
	{
		if(firstLoop)
		{
			firstLoop = false;
		}
		else
		{
			output << ", ";
		}

		output << item;
	}

	output << ']';

	return output.str();
}


// template function for generating random vectors of different types of integer.
// Takes parameters for the count, seed, min and max, and whether or not duplicate values are allowed.
template<typename IntType>
std::vector<IntType> makeRandomNumberVector(size_t count, IntType min, IntType max, RandomSeed seed, bool allowDuplicates)
{
	// set up random number generator
	std::mt19937 randEngine;
	randEngine.seed(seed);

	std::uniform_int_distribution<IntType> distributor(min, max);

	// generate the vector
	std::vector<IntType> randomVector;
	randomVector.reserve(count);
	std::set<IntType> usedValues;
	do
	{
		IntType randInt = distributor(randEngine);

		if(!allowDuplicates)
		{
			// skip value if it's a duplicate

			if(usedValues.find(randInt) != usedValues.end())
			{
				//duplicate
				continue;
			}

			usedValues.insert(randInt);
		}

		randomVector.push_back(randInt);
	}
	while(randomVector.size() < count);


	return randomVector;
}

// template function for generating a random integer
// Takes parameters for the seed, min and max.
template<typename IntType>
IntType makeRandomNumber(IntType min, IntType max, RandomSeed seed)
{
	// set up random number generator
	std::mt19937 randEngine;
	randEngine.seed(seed);

	std::uniform_int_distribution<IntType> distributor(min, max);

	return distributor(randEngine);
}

// returns a set containing the in set1 but not set2
template<typename T>
std::set<T> subtractSet(const std::set<T> & set1, const std::set<T> & set2)
{
	// start with a set of all elements in set1
	std::set<T> result = set1;

	typename std::set<T>::iterator resultIter = result.begin();

	// and prune out everything in set2
	while(resultIter != result.end())
	{
		if(set2.find(*resultIter) != set2.end())
		{
			resultIter = result.erase(resultIter);
		}
		else
		{
			++resultIter;
		}
	}

	return result;
}

#endif //CS104FINAL_GROCERY_TESTING_UTILS_H
