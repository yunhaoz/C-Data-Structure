#include <vector>

/**
 * Performs mergesort on the provided vector using
 * the Compare object, comp, to peform ALL comparisons
 *
 * Must run in O(n*log(n))
 */
template <class T, class Compare >
void mergeSort(std::vector<T>& list, Compare comp);

template<class T, class Compare>
void mergeSort(std::vector<T>& list, Compare comp)
{
    if (list.size() <= 1)
    {
        return;
    }
    int mid = list.size() / 2;
    vector<T> left;
    vector<T> right;
    for (size_t i = 0; i < mid; i++)
        left.push_back(list[i]);
    for (size_t j = mid; j < list.size(); j++)
        right.push_back(list[j]);
    mergeSort(left, comp);
    mergeSort(right, comp);
    mymergeSort(left, right, list, comp);
}

template<class T, class Compare>
void mymergeSort(std::vector<T>& left, std::vector<T>& right, std::vector<T>& list, Compare comp);

template<class T, class Compare>
void mymergeSort(std::vector<T>& left, std::vector<T>& right, std::vector<T>& list, Compare comp)
{
    int leftsize = left.size();
    int rightsize = right.size();
    int l = 0;
    int r = 0;
    int i = 0;
    while (l < leftsize && r < rightsize)
    {
        if (comp(left[l], right[r]))
        {
            list[i] = left[l];
            l++;
        }
        else
        {
            list[i] = right[r];
            r++;
        }
        i++;
    }
    while (l < leftsize)
    {
        list[i] = left[l];
        l++;
        i++
    }
    while (l < leftsize)
    {
        list[i] = right[r];
        r++;
        i++
    }
}