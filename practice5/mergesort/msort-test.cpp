// You may use this test-file as you like. It has helper functions
// to help create random arrays and verify sorted order. You do NOT
// need to use this file if you do NOT like.  You may make a separate
// GTEST program, or other tests.  Submit what ever test code you
// create so we can review it (but it won't be graded).
#include "msort.h"
#include <iostream>
#include <vector>
#include <functional>
#include <cstdlib>

using namespace std;

// Prototypes for helper test functions
template <typename T, typename Comp>
bool verifySorted(const std::vector<T>& vec, const Comp& c);

void genRandIntVector(std::vector<int>& vec, size_t numRandElems);


int main()
{
    // Create some test vector, sort them with your mergesort function
    // and then verify they are sorted.



    return 0;
}

/**
 * Returns true if the vector is sorted according to the
 * given comparator.
 *
 * Comp should be a functor implementing a less-than check
 */
template <typename T, typename Comp>
bool verifySorted(const std::vector<T>& vec, const Comp& c)
{
    if(2U > vec.size()) return true;

    // Vector has at least 2 elements
    for(size_t i = 0; i < vec.size()-1; i++) {
        if(c(vec[i+1], vec[i])) return false;
    }
    return true;
}

void genRandIntVector(std::vector<int>& vec, size_t numRandElems)
{
    vec.clear();
    vec.resize(numRandElems);
    srand(time(0));
    for(size_t i = 0; i < numRandElems; i++)
    {
        vec[i] = rand() % 10000;
    }
}
