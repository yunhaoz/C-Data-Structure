/*
 * GROCERY PROVIDED TEST FILE
 * --------------------------------------------------
 *
 * We have given you a number of simple test cases to help verify that your program works.
 * These do NOT cover all possible combinations of inputs, but they should be enough to get you started.
 * Add more tests if you can!  Just copy the structure of the existing ones; it should be fairly clear
 * how the checkGroceryBox() function works.  Hope you remember initializer lists from the C++11 lab ;-).
 * Tests are not graded, but they will help you get a better grade on the problem!
 */

#include "testing-utils.h"

/**
 * Does the program run on basic input without crashing?
 */
TEST(SuperSimple, RunsWithoutCrashing)
{
	std::vector<GItem> items = { GItem(1, 5, 3), GItem(2, 2, 2), GItem(3, 1, 1), GItem(4, 1, 2) };
	EXPECT_NO_THROW(maxGroceryBox(items, 4));
}

/**
 * Get an item.  Return the item intact. Easy.
 */
TEST(SuperSimple, OneItemReturned)
{
	std::vector<GItem> items = { GItem(2, 3, 2) };
	std::vector<GItem> result = maxGroceryBox(items, 6);

	EXPECT_TRUE(checkGroceryBox(items, result));
}

/**
 * Simple test case where the program can choose only one item.
 */
TEST(WeightLimit, OneItem)
{
	std::vector<GItem> items = { GItem(1, 5, 3), GItem(2, 2, 2) };
	std::vector<GItem> result = maxGroceryBox(items, 3);

	EXPECT_TRUE(checkGroceryBox({ GItem(2, 2, 2)}, result));
}

/**
 * Simple test case where the program can choose no items.
 */
TEST(WeightLimit, NoItems)
{
	std::vector<GItem> items = { GItem(1, 5, 3), GItem(2, 2, 2) };
	std::vector<GItem> result = maxGroceryBox(items, 1);

	EXPECT_TRUE(checkGroceryBox({ }, result));
}

/**
 * This has one large item and then two medium items.  Can the program realize that the two medium options
 * are the better choice?
 */
TEST(CorrectSelection, FindsBetterSolutionAtEnd)
{
	std::vector<GItem> items = { GItem(1, 5, 4), GItem(2, 3, 2), GItem(3, 3, 3) };
	std::vector<GItem> result = maxGroceryBox(items, 7);

	EXPECT_TRUE(checkGroceryBox({ GItem(3, 3, 3), GItem(2, 3, 2) }, result));
}

/**
 * Now it has to combine 3 items to get a better solution.
 */
TEST(CorrectSelection, FindsBetter3Items)
{
	std::vector<GItem> items = { GItem(1, 5, 3), GItem(2, 2, 2), GItem(3, 1, 1), GItem(4, 2, 2) };
	std::vector<GItem> result = maxGroceryBox(items, 6);

	EXPECT_TRUE(checkGroceryBox({ GItem(2, 2, 2), GItem(3, 1, 1), GItem(4, 2, 2) }, result));
}

/**
 * Can your program handle having no items at all?
 */
TEST(BoundaryConditions, NoItems)
{
	std::vector<GItem> items = {  };
	std::vector<GItem> result = maxGroceryBox(items, 6);

	EXPECT_TRUE(checkGroceryBox(items, result));
}
