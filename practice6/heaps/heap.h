#ifndef HEAP_H
#define HEAP_H
#include <functional>
#include <stdexcept>
#include <vector>
#include <algorithm>

template <typename T, typename PComparator = std::less<T> >
class Heap
{
public:
    /// Constructs an m-ary heap for any m >= 2
    Heap(int m, PComparator c = PComparator());

    /// Destructor as needed
    ~Heap();

    /// Adds an item
    void push(const T& item);
    void trickelUp(int loc);

    /// returns the element at the top of the heap
    ///  max (if max-heap) or min (if min-heap)
    T const& top() const;

    /// Removes the top element
    void pop();
    void heapify(int index);

    /// returns true if the heap is empty
    bool empty() const;

private:
    /// Add whatever helper functions and data members you need below
    std::vector<T> myheap;
    PComparator comp;
    int mchild;
};

// Add implementation of member functions here
template <typename T, typename PComparator>
Heap<T, PComparator>::Heap(int m, PComparator c)
{
    comp = c;
    mchild = m;
}

template <typename T, typename PComparator>
Heap<T, PComparator>::~Heap()
{
}

template <typename T, typename PComparator>
void Heap<T, PComparator>::push(const T& item)
{
    myheap.push_back(item);
    int loc = myheap.size() - 1;
    trickelUp(loc);
}

//helper function of push
template <typename T, typename PComparator>
void Heap<T, PComparator>::trickelUp(int loc)
{
    int parent = (loc - 1) / mchild;
    while (parent >= 0 && comp(myheap[loc], myheap[parent]))
    {
        std::swap(myheap[loc], myheap[parent]);
        loc = parent;
        parent = (parent - 1) / mchild;
    }
}

// We will start top() for you to handle the case of
// calling top on an empty heap
template <typename T, typename PComparator>
T const & Heap<T,PComparator>::top() const
{
    // Here we use exceptions to handle the case of trying
    // to access the top element of an empty heap
    if(empty()) {
        throw std::logic_error("can't top an empty heap");
    }
    // If we get here we know the heap has at least 1 item
    // Add code to return the top element
    return myheap[0];
}


// We will start pop() for you to handle the case of
// calling top on an empty heap
template <typename T, typename PComparator>
void Heap<T,PComparator>::pop()
{
    if(empty()) {
        throw std::logic_error("can't pop an empty heap");
    }
    myheap[0] = myheap.back();
    myheap.pop_back();
    heapify(0);
}

//helper function of pop
template <typename T, typename PComparator>
void Heap<T, PComparator>::heapify(int index)
{
    //if is the leaf node
    if ((index * mchild + 1) > (myheap.size() - 1))
    {
        return;
    }
    int smallerChild = index * mchild + 1;// start with the left child
    for (int i = 1; i <= mchild; i++)//compare all the child
    {
        int currChild = index * mchild + i;
        if (currChild < myheap.size())//if the curr child exist
        {
            if (comp(myheap[currChild], myheap[smallerChild]))
            {
                smallerChild = currChild;//find the smaller child
            }
        }
    }
    if (comp(myheap[smallerChild], myheap[index]))
    {
        std::swap(myheap[smallerChild], myheap[index]);
        heapify(smallerChild);
    }
}


#endif

