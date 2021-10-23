//Yunhao Zhao
// You must provide your implementation of maxGroceryBox() and all
// helper functions in this file.
//

#include "grocery.h"

// You may prototype helper functions here and implement them below

void maxGroceryBoxHelper(const std::vector<GItem>& inventory, std::vector<GItem>& result, std::vector<GItem>& temp,
    int& max_value, int curr_value, int curr_weight, int weight_limit, int index)
{
    //update the max result
    if (index == inventory.size())
    {
        return;
    }
    if (curr_weight + inventory[index].weight > weight_limit)
    {
        return;
    }
    else
    {
        for (int i = index; i < inventory.size(); i++)
        {
            temp.push_back(inventory[index]);
            if (curr_value > max_value)
            {
                result = temp;
                max_value = curr_value;
            }
            maxGroceryBoxHelper(inventory, result, temp, max_value, curr_value + inventory[index].value, curr_weight + inventory[index].weight, weight_limit, index + 1);
            temp.pop_back();
        }
    }
}

std::vector<GItem> maxGroceryBox(const std::vector<GItem>& inventory, int weight_limit)
{
    // TODO implement!
    vector<GItem> temp;
    vector<GItem> result;
    maxGroceryBoxHelper(inventory, result, temp, 0, 0, 0, weight_limit, 0);
    return result;
}

