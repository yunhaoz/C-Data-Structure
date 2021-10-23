/*
 * TRIE PROVIDED TEST FILE
 * --------------------------------------------------
 *
 * We have given you a number of simple test cases to help verify that your trie works.
 * These do NOT cover all possible combinations of inputs, but they should be enough to get you started.
 *
 * As you'll notice, we are testing your insert() and your contains() separately.  This is good for you
 * because it means that errors in one won't cascade over to the other, but it also means that your
 * implementation needs to be compatible with ours.  These tests will help you check that.
 *
 * Feel free to add more tests if you want!  Just copy the structure of the existing ones; hopefully
 * it shouldn't be too hard to understand how they work.
 * Tests are not graded, but they will help you get a better grade on the problem!
 */

#include "trie.h"

#include "trie-test-functions.h"

#include <gtest/gtest.h>

/*
 * Insert the empty string and make sure it's in the trie.
 */
TEST(InsertEmpty, OneInsertion)
{
	SparseTrie trie;
	trie.insert("");

	EXPECT_EQ(nullptr, trie.getRoot()->next);
	EXPECT_EQ(nullptr, trie.getRoot()->children);
	EXPECT_EQ(true, trie.getRoot()->terminal);

}

/*
 * Inserts one letter and checks that it can be found in the trie.  Only checks the node value.
 */
TEST(InsertOneLevel, OneInsertionValue)
{
	SparseTrie trie;
	trie.insert("c");

	EXPECT_EQ(nullptr, trie.getRoot()->next);
	ASSERT_NE(nullptr, trie.getRoot()->children);

	EXPECT_EQ('c', trie.getRoot()->children->letter);
}

/*
 * Inserts one letter and checks that the sibling and child pointers are set correctly.
 */
TEST(InsertOneLevel, ChildPointers)
{
	SparseTrie trie;
	trie.insert("c");

	ASSERT_NE(nullptr, trie.getRoot()->children);

	EXPECT_EQ(nullptr, trie.getRoot()->children->next);
	EXPECT_EQ(nullptr, trie.getRoot()->children->children);
}

/*
 * Inserts one letter and checks that it can be found in the trie.
 */
TEST(InsertOneLevel, OneInsertion)
{
	SparseTrie trie;
	trie.insert("c");

	EXPECT_TRUE(checkTrieNodeChildren({{'c', true}}, trie.getRoot()));
}

/*
 * Inserts two three letter words that diverge at the second letter.  The words should share the top-level node
 * 'c' then split into two.
 */
TEST(InsertMultiLevel, DifferAtSecondLetter)
{
	SparseTrie trie;
	trie.insert("car");
	trie.insert("cot");


	EXPECT_TRUE(checkTrieNodeChildren({{'c', false}}, trie.getRoot()));
	EXPECT_TRUE(checkTrieNodeChildren({{'a', false}, {'o', false}}, trie.getRoot()->children));

	// find the child nodes
	TNode * oNode;
	TNode * aNode;
	if(trie.getRoot()->children->children->letter == 'a')
	{
		aNode = trie.getRoot()->children->children;
		oNode = aNode->next;
	}
	else
	{
		oNode = trie.getRoot()->children->children;
		aNode = oNode->next;
	}

	EXPECT_TRUE(checkTrieNodeChildren({{'r', true}}, aNode));
	EXPECT_TRUE(checkTrieNodeChildren({{'t', true}}, oNode));
}

/**
 * Inserts a long word first, then a shorter version of it.
 */
TEST(InsertExtension, LongThenShort)
{
	SparseTrie trie;
	trie.insert("aint");
	trie.insert("ai");

	EXPECT_TRUE(checkTrieNodeChildren({{'a', false}}, trie.getRoot()));
	EXPECT_TRUE(checkTrieNodeChildren({{'i', true}}, trie.getRoot()->children));
	EXPECT_TRUE(checkTrieNodeChildren({{'n', false}}, trie.getRoot()->children->children));
	EXPECT_TRUE(checkTrieNodeChildren({{'t', true}}, trie.getRoot()->children->children->children));
}

/*
 * This test checks that the empty string is handled properly
 */
TEST(ContainsEmpty, EmptyString)
{
	SparseTrie trie;

	EXPECT_FALSE(trie.contains(""));

	trie.getRoot()->terminal = true;

	EXPECT_TRUE(trie.contains(""));
}

/*
 * This checks for a one-character word that doesn't exist in an empty tree
 */
TEST(ContainsEmpty, NonExistantLetter)
{
	SparseTrie trie;

	EXPECT_FALSE(trie.contains("r"));
}

/*
 * This creates a single node and checks that it can be found
 */
TEST(ContainsOneLevel, OneWord)
{
	SparseTrie trie;

	trie.getRoot()->children = new TNode('r', true, nullptr);

	EXPECT_TRUE(trie.contains("r"));
}

/*
 * This creates multiple nodes and checks that they can all be found
 */
TEST(ContainsOneLevel, MultiWord)
{
	SparseTrie trie;

	TNode * xNode = new TNode('x', true, nullptr);
	TNode * aNode = new TNode('a', true, xNode);

	trie.getRoot()->children = new TNode('r', true, aNode);

	EXPECT_TRUE(trie.contains("r"));
	EXPECT_TRUE(trie.contains("a"));
	EXPECT_TRUE(trie.contains("x"));
}

/*
 * This inserts a node structure for the words "car" and "cot" and checks that they can be found
 */

TEST(ContainsMultiLevel, DifferAtSecondLetter)
{
	SparseTrie trie;

	TNode * carNode = new TNode('r', true );
	TNode * caNode = new TNode('a', false);
	caNode->children = carNode;

	TNode * cotNode = new TNode('t', true);
	TNode * coNode = new TNode('o', false);
	coNode->children = cotNode;
	coNode->next = caNode;

	TNode * cNode = new TNode('c', false);
	cNode->children = coNode;

	trie.getRoot()->children = cNode;

	EXPECT_TRUE(trie.contains("car"));
	EXPECT_TRUE(trie.contains("cot"));

	EXPECT_FALSE(trie.contains("cat"));
	EXPECT_FALSE(trie.contains("cor"));

}
