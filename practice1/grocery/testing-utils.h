//
// This file contains some helper code for our test suite.
// You shouldn't need to mess with it.
//

#ifndef CS104FINAL_GROCERY_TESTING_UTILS_H
#define CS104FINAL_GROCERY_TESTING_UTILS_H

#include "grocery.h"

#include <string>
#include <vector>
#include <sstream>
#include <set>
#include <random>

#include <gtest/gtest.h>

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

typedef uint32_t RandomSeed;

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

/**
 * Comparator that sorts items in order of decreasing weight.
 */
struct GroceryItemComparator
{
	bool operator()(GItem const & lhs, GItem const & rhs)
	{
		if(lhs.weight != rhs.weight)
		{
			return lhs.weight > rhs.weight;
		}
		else if(lhs.value != rhs.value)
		{
			return lhs.value > rhs.value;
		}
		else
		{
			return lhs.id > rhs.id;
		}
	}
};

// operator for comparing GroceryItems as equal
inline bool operator==(GItem const & lhs, GItem const & rhs)
{
	return (lhs.id == rhs.id) && (lhs.weight == rhs.weight) && (lhs.value == rhs.value);
}

/**
 * Check the result of the grocery box algorithm against an expected box.
 * Note: elements of expected and actual can be in any order.
 * @param expected
 * @param actual
 * @return
 */
inline testing::AssertionResult checkGroceryBox(std::vector<GItem> expected, std::vector<GItem> actual)
{
	// Convert vectors to sets so it's easier to check that they contain the same things
	std::set<GItem, GroceryItemComparator> expectedSet(expected.begin(), expected.end());
	std::set<GItem, GroceryItemComparator> actualSet(actual.begin(), actual.end());

	if(actual.size() != actualSet.size())
	{
		return testing::AssertionFailure() << "Item vector contains duplicates!  Item vector returned was: " << vectorToString(actual);
	}

	if(expectedSet != actualSet)
	{
		return testing::AssertionFailure() << "Incorrect grocery box!  " << std::endl << "Expected: " << setToString(expectedSet) << std::endl << "Actual: " << setToString(actualSet) << std::endl;
	}

	return testing::AssertionSuccess();
}

/**
 * Check the result of the grocery box algorithm against multiple possible expected boxes.
 * Note: elements of expected and actual can be in any order.
 * @param expected
 * @param actual
 * @return
 */
inline testing::AssertionResult checkGroceryBoxMultiSolution(std::vector<std::vector<GItem>> possibleExpectedBoxes, std::vector<GItem> actual)
{
	std::set<GItem, GroceryItemComparator> actualSet(actual.begin(), actual.end());

	if(actual.size() != actualSet.size())
	{
		return testing::AssertionFailure() << "Item vector contains duplicates!  Item vector returned was: " << vectorToString(actual);
	}

	for(std::vector<GItem> const & expected : possibleExpectedBoxes)
	{
		std::set<GItem, GroceryItemComparator> expectedSet(expected.begin(), expected.end());

		if(expectedSet == actualSet)
		{
			// found one!
			return testing::AssertionSuccess();
		}
	}

	testing::AssertionResult errorMessage = testing::AssertionFailure();
	errorMessage << "Incorrect grocery box! Expected one of these boxes: " << std::endl;
	for(std::vector<GItem> const & expected : possibleExpectedBoxes)
	{
		std::set<GItem, GroceryItemComparator> expectedSet(expected.begin(), expected.end());
		errorMessage << "   " << setToString(expectedSet) << std::endl;
	}
	errorMessage << "Actual: " << setToString(actualSet) << std::endl;
	return errorMessage;
}

#endif //CS104FINAL_GROCERY_TESTING_UTILS_H
