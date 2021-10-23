#ifndef TODOLIST_H
#define TODOLIST_H

#include <string>
/**
 * Daily To-Do List Class storing a list of to-do items per day.
 * Days are indexed 0 to N-1 and any number of items can be in
 * a given day's to-do list.  We also support high-priority items
 * via a high priority list where items are members of a daily
 * to do list AND the high priority list.  Values are not duplicated
 * in the priority list but shared between the lists.
 *
 * Each day's to-do list and the high priority list will be 
 * implemented as a singly-linked list.  Each list (each day) will be
 * an entry in a dynamically resizing array.  Each index in this array
 * corresponds to the index of a particular day. 
 */
class DailyTodoList
{
public:

    // This struct will be used for items in daily to-do lists
    // and the high priority list.  It is nested inside the scope
    // of the DailyTodoList because this struct really only applies
    // to this class.  In function prototypes or place where you are
    // not inside the scope of DailyTodoList, you will need to scope
    // this struct name to DailyTodoList::Item.
    struct Item {
        std::string val;
        Item* nextItem;
        Item* nextPriorityItem;
        Item(const std::string& v, Item* ni, Item* np)
        {
            val = v; nextItem = ni; nextPriorityItem = np;
        }
    };

    /**
     *  Default Constructor 
     */
    DailyTodoList();
    /**
     *  Destructor
     */
    ~DailyTodoList();

    /**
      *  Inserts a new value into the end of the given day's to-do list.
      *  [COMPLETED]
      *
      *  @param[in] dayIndex
      *    Day index of the to-do list that val should be inserted into.
      *    If dayIndex is beyond the current size of the data array, it should
      *    be resized and the items transferred to the new larger data array.
      *  @param[in] val
      *    String value to insert
      *  @param[in] highPriority
      *    True if this value should also be added to the end of the high
      *    priority list
      *  @throws std::bad_alloc
      *    Memory could not be allocated
      *
      *  Required complexity: Should run in worst-case time O(n+k)
      *       where n is the size of the data array and k is the length of
      *       the given day's to do list.
      *       *However*, if a sequence of inserts is performed on sequential
      *       days (d, d+1, d+2, ...) the amortized runtime of insert should be
      *       O(k) where k is the average length of the to-do lists for given days.
      */
    void push_back(size_t dayIndex, const std::string& val, bool highPriority);

    /**
     *  Inserts a new value into the given day's to-do list
     *  at the desired location. 
     *  [TO BE WRITTEN]
     *
     *  @param[in] dayIndex
     *    Day index of the to-do list that val should be inserted into.
     *    If dayIndex is beyond the current size of the data array, it should 
     *    be resized and the items transferred to the new larger data array.
     *  @param[in] loc
     *    Location to insert the new value in the given day's to-do list 
     *  @param[in] val
     *    String value to insert
     *  @param[in] highPriority
     *    True if this value should also be added to the end of the high
     *    priority list
     *  @throws std::out_of_range
     *    If the location is beyond the end of the given to-do list.  
     *  @throws std::bad_alloc
     *    Memory could not be allocated 
     *
     *  Required complexity: Should run in worst-case time O(n+loc)
     *       where n is the size of the data array and loc is the index at
     *       which to insert in the given day's to do list.
     *       *However*, if a sequence of insertions is performed on sequential
     *       days (d, d+1, d+2, ...) to locations (l, l', l'') then the amortized
     *       runtime of insert should be O(average of l, l', l'', etc).
     */
    void insert(size_t dayIndex, size_t loc, const std::string& val, bool highPriority);

    /**
     *  Removes the value at the specified location in the given day's 
     *  to-do list
     *  [TO BE WRITTEN]
     *
     *  @param[in] dayIndex
     *    Day index of the to-do list to remove from
     *  @param[in] loc
     *    Location to remove in the given day's to-do list 
     *  @throws std::out_of_range
     *    If the location is invalid.
     *
     *  Required complexity: Should run in time O(loc+p) where n is the size of 
     *  the data array and loc is the length of the to-do list for the given day,
     *  and p is the length of the high priority list.
     */
    void remove(size_t dayIndex, size_t loc);

    /**
     *  Returns the number of days / to-do lists stored
     *  Days range from 0 to n-1 and represent the size/capacity
     *  of the array storing the to-do lists for each day.
     *  [TO BE WRITTEN]
     *
     *  Required Complexity: O(1)
     */
    size_t numDays() const;

    /**
     *  Returns the number of items in the to-do list for the given day
     *  [TO BE WRITTEN]
     *
     *  @param[in] dayIndex
     *    Index of the day to return the number of to-do list items
     *  @throws std::out_of_range
     *    If the dayIndex is invalid.
     *
     *  Required Complexity: O(k) where k is the length of the to-do list
     *                       for the given day
     */
    size_t numItemsOnDay(size_t dayIndex) const;

    /**
     *  Returns true if the to-do list for the given day is empty
     *  [TO BE WRITTEN]
     *
     *  @param[in] dayIndex
     *    Index of the desired to-do list 
     *  @returns true if the to-do list for the given day is empty,
     *     and false otherwise.
     *  @throws std::out_of_range
     *    If the dayIndex is invalid.
     *
     *  Required Complexity: O(1) 
     */
    bool empty(size_t dayIndex) const;

    /**
     *  Returns the to-do value for the given day and location in that
     *  day's to-do list.
     *  [TO BE WRITTEN]
     *
     *  @param[in] dayIndex
     *    Index of the day's to-do list 
     *  @param[in] loc
     *    Location of the item to get from the given day's to-do list
     *  @returns the to-do value
     *  @throws std::out_of_range
     *    If the dayIndex is invalid.
     *
     *  Required Complexity: O(loc) 
     */
    const std::string& getNormalVal(size_t dayIndex, size_t loc) const;
    std::string& getNormalVal(size_t dayIndex, size_t loc);

    /**
     *  Returns the number of values in the high priority list
     *  [TO BE WRITTEN]
     *
     *  Required Complexity: O(1)
     */
    size_t numPriorityItems() const;

    /**
     *  Returns the to-do value for the given location in the high 
     *  priority list.
     *  [TO BE WRITTEN]
     *
     *  @param[in] priorityLoc
     *    Location of the item to get from the high priority list
     *  @returns the to-do value
     *  @throws std::out_of_range
     *    If priorityLoc is invalid.
     *
     *
     *  Required Complexity: O(priorityLoc)
     */
    const std::string& getPriorityVal(size_t priorityLoc) const;
private:
    //*********************** Private helpers ********************************
    // Consider adding appropriate helper functions for specific or 
    // repetitive sub-tasks
    Item* accessHighP(size_t k);
    void removePrior(const std::string& val);
    
    //************************* Data members *********************************
    // Pointer to the array storing the head pointers to each day's
    // to-do list
    Item** data_;
    // Capacity of the array storing the head pointers
    size_t cap_;
    // Head pointer to high priority items list
    Item* priorityHead_;
    // Feel free to add appropriate scalar (individual pointer/integral) data 
    // members to increase efficiency. You may NOT add composite data 
    // structures like arrays, vectors, lists, maps, etc. 
    size_t numOfhpl;


};

#endif

