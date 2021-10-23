#ifndef GROCERY_H
#define GROCERY_H

//-----------------------------------------------------------------------------
//  YOU SHOULD NOT CHANGE THIS FILE.
//   - PROVIDE YOUR IMPLEMENTATION AND HELPER FUNCTIONS IN grocery.cpp
//-----------------------------------------------------------------------------
#include <vector>
#include <iostream>

// Struct to represent a grocery item
struct GItem {
    int id;     // unique id to represent an item
    int weight; // weight of the item (always 1 or more)
    int value;  // monetary value of the item (always 1 or more)

    GItem(int _id, int _weight, int _value): id(_id), weight(_weight), value(_value)
    {}
};

// Convenience function for printing a grocery item
inline std::ostream& operator<<(std::ostream& ostr, GItem const & gi) {
    ostr << gi.id << ":" << gi.weight << ":" << gi.value;
    return ostr;
}

/**
 * By using each inventory item at most once, find the items
 * that can be packed in a box with at most 'weight_limit' weight
 * and maximizes the total value of items in the box.  Return the
 * items that yield the maximum value box in a vector.
 *
 * If there are multiple ways to pack the box that cause a tie
 * in total value, you may return *ANY* of those packings. Essentially,
 * do not worry about ties.
 *
 * You may assume:
 * - There are no items with the same ID
 * - Each item has weight of at least 1
 * - Each item has value of at least 1
 *
 * @param[in] inventory
 *   The list of items that may be packed in the box.
 * @param[in] weight_limit
 *   Maximum total weight of items that the box can hold
 *
 * @return
 *   Vector of items that meet the weight limit and yield the max
 *   total value.
 */
std::vector<GItem> maxGroceryBox(const std::vector<GItem>& inventory, int weight_limit);

#endif
