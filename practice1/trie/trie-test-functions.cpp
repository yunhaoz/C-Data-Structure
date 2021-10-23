//
// Utility functions for our trie testing.
// You should not need to modify this file.
//

#include "trie-test-functions.h"

testing::AssertionResult checkTrieNodeChildren(std::map<char, bool> expectedElements, TNode * parentNode)
{
	std::set<char> extraNodesFound;

	// iterate across the level and keep track of the nodes we find.
	TNode * currNode = parentNode->children;
	while(currNode != nullptr)
	{
		auto expectedIter = expectedElements.find(currNode->letter);
		if(expectedIter == expectedElements.end())
		{
			extraNodesFound.insert(expectedIter->first);
		}
		else
		{
			if(expectedIter->second != currNode->terminal)
			{
				return testing::AssertionFailure() << std::boolalpha << "The node " << currNode->letter << " has an incorrect terminal flag.  It is supposed to be " << expectedIter->second << " but is actually " << currNode->terminal << ".";
			}

			expectedElements.erase(expectedIter);
		}

		currNode = currNode->next;
	}

	if(!expectedElements.empty())
	{
		testing::AssertionResult failMessage = testing::AssertionFailure();
		failMessage << "Missing elements at this trie level: [";
		bool firstLoop = true;
		for(auto const & element : expectedElements)
		{
			if(firstLoop)
			{
				firstLoop = false;
			}
			else
			{
				failMessage << ", ";
			}

			failMessage << element.first;
		}
		failMessage << "]";
		return failMessage;
	}

	if(!extraNodesFound.empty())
	{
		return testing::AssertionFailure() << "Extra elements found at this trie level: " << setToString(extraNodesFound);
	}

	return testing::AssertionSuccess();
}

std::vector<std::string> makeRandomWordVector(size_t count, RandomSeed seed, size_t maxLength, bool allowDuplicates)
{
	// set up random number generator
	std::mt19937 randEngine;
	randEngine.seed(seed);

	// have each byte be a character between a and z
	std::uniform_int_distribution<uint8_t> charDist('a', 'z');
	std::uniform_int_distribution<size_t> lengthDist(0, maxLength);

	// generate the vector
	std::vector<std::string> randomVector;
	randomVector.reserve(count);
	std::set<std::string> usedValues;
	do
	{
		std::string currString;
		size_t stringLength = lengthDist(randEngine);

		while(currString.length() < stringLength)
		{
			currString.insert(currString.length(), 1, charDist(randEngine));
		}

		if(!allowDuplicates)
		{
			// skip value if it's a duplicate

			if(usedValues.find(currString) != usedValues.end())
			{
				//duplicate
				continue;
			}

			usedValues.insert(currString);
		}

		randomVector.push_back(currString);
	}
	while(randomVector.size() < count);

	return randomVector;
}