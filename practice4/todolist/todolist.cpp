#include <stdexcept>
#include "todolist.h"
using namespace std;


DailyTodoList::DailyTodoList()
{
    data_ = new Item * [5];
    cap_ = 5;
    priorityHead_ = NULL;
    // [TO BE COMPLETED]
    //===========================
    // Add any addition code here
    numOfhpl = 0;
    
}

DailyTodoList::~DailyTodoList()
{
    for (size_t i = 0; i < cap_; i++)
    {
        for (size_t j = numItemsOnDay(i)-1;j>=0; j--)
        {
            remove(i, j);
        }
        delete data_[i];
    }
    delete[] data_;
}



void DailyTodoList::push_back(size_t dayIndex, const std::string& val, bool highPriority)
{
    if (dayIndex < cap_)
    {
        this->insert(dayIndex, this->numItemsOnDay(dayIndex), val, highPriority);
    }
    else 
    {
        this->insert(dayIndex, 0, val, highPriority);
    }
    
}

void DailyTodoList::insert(size_t dayIndex, size_t loc, const std::string& val, bool highPriority)
{
    if (dayIndex >= cap_)
    {
        Item** newdata_ = new Item * [dayIndex * 2];
        for (int i = 0; i < cap_; i++)
        {
            newdata_[i] = data_[i];
        }
        delete[] data_;
        data_ = newdata_;
        cap_ = 2 * cap_;
    }
    if (numItemsOnDay(dayIndex) < loc)
    {
        throw std::out_of_range("Out of the range of to-do-list");
    }
    else
    {
        size_t count = 0;
        if (empty(dayIndex))
        {
            if (highPriority)
            {
                Item* temp = new Item(val, NULL, priorityHead_);
                priorityHead_ = temp;
                data_[dayIndex] = temp;
                numOfhpl++;
            }
            else
            {
                Item* temp = new Item(val, NULL, NULL);
                data_[dayIndex] = temp;
            }
            return 0;
        }
        Item* curr = data_[dayIndex];
        while (curr->nextItem != NULL || count != loc)
        {
            curr = curr->nextItem;
            count++;
        }
        if (highPriority)
        {
            Item* temp = new Item(val, curr->nextItem, priorityHead_);
            priorityHead_ = temp;
            if (loc == 0)
            {
                data_[dayIndex] = temp;
            }
            else
            {
                curr->nextItem = temp;
            }
            numOfhpl++;
        }
        else
        {
            Item* temp = new Item(val, curr->nextItem, NULL);
            if (loc == 0)
            {
                data_[dayIndex] = temp;
            }
            else
            {
                curr->nextItem = temp;
            }
        }
    }
}

void DailyTodoList::remove(size_t dayIndex, size_t loc)
{
    if (dayIndex >= cap_)
    {
        throw std::out_of_range("Out of the array");
    }
    else
    {
        if (numItemsOnDay(dayIndex) <= loc)
        {
            throw std::out_of_range("Invalid remove");
        }
        else
        {
            size_t count = 0;
            Item* curr = data_[dayIndex];
            Item* prev = curr;
            while (count != loc)
            {
                prev = curr;
                curr = curr->nextItem;
                count++;
            }
            if (loc = 0)
            {
                data_[dayIndex] = curr->nextItem;
                delete curr;
            }
            else
            {
                prev->nextItem = curr->nextItem;
                delete curr;
            }
        }
    }
}

size_t DailyTodoList::numDays() const
{
    return cap_;
}

size_t DailyTodoList::numItemsOnDay(size_t dayIndex) const
{
    size_t count = 0;
    if (dayIndex >= cap_)
    {
        throw std::out_of_range("Out of Range");
    }
    else
    {
        Item* temp = data_[dayIndex];
        while (temp != NULL)
        {
            count++;
            temp = temp->nextItem;
        }
    }
    return count;
}

bool DailyTodoList::empty(size_t dayIndex) const
{
    if (dayIndex >= cap_)
    {
        throw std::out_of_range("Out of Range");
    }
    else
    {
        if (data_[dayIndex] == NULL)
        {
            return true;
        }
        else
        {
            return false;
        }
    }
}

const std::string& DailyTodoList::getNormalVal(size_t dayIndex, size_t loc) const
{
    size_t count = 0;
    if (dayIndex >= cap_)
    {
        throw std::out_of_range("Out of Range");
    }
    else
    {
        Item* temp = data_[dayIndex];
        while (temp != NULL)
        {
            count++;
            if (count == loc)
            {
                return temp->val;
            }
            temp = temp->nextItem;
        }
    }
    return 0;
}

std::string& DailyTodoList::getNormalVal(size_t dayIndex, size_t loc)
{
    size_t count = 0;
    if (dayIndex >= cap_)
    {
        throw std::out_of_range("Out of Range");
    }
    else
    {
        Item* temp = data_[dayIndex];
        while (temp != NULL)
        {
            count++;
            if (count == loc)
            {
                return temp->val;
            }
            temp = temp->nextItem;
        }
    }
    return 0;
}

size_t DailyTodoList::numPriorityItems() const
{
    return numOfhpl;
}

const std::string& DailyTodoList::getPriorityVal(size_t priorityLoc) const
{
    size_t count = 0;
    if (priorityLoc >= numOfhpl)
    {
        throw std::out_of_range("Out of Range.");
    }
    else
    {
        Item* temp = priorityHead_;
        while (temp != NULL)
        {
            count++;
            if (count == priorityLoc)
            {
                return temp->val;
            }
            temp = temp->nextItem;
        }
    }
}

DailyTodoList::Item* DailyTodoList::accessHighP(size_t k)
{
    size_t count = 0;
    if (k >= numOfhpl)
    {
        throw std::out_of_range("Out of Range");
    }
    else
    {
        Item* temp = priorityHead_;
        while (count != k)
        {
            count++;
            temp = temp->nextPriorityItem;
        }
        return temp;
    }
}

void DailyTodoList::removePrior(const std::string& val)
{
    Item* priorTemp = priorityHead_;
    size_t priorK = 0;
    while (priorTemp != NULL)
    {
        if(priorTemp->val==val)
        {
            if (priorK = 0)
            {
                priorityHead_ = priorTemp->nextPriorityItem;
                delete priorTemp;
            }
            else
            {
                accessHighP(priorK - 1)->nextPriorityItem = priorTemp->nextPriorityItem;
                delete priorTemp;
            }
        }
        priorK++;
        priorTemp = priorTemp->nextPriorityItem;
    }
}
