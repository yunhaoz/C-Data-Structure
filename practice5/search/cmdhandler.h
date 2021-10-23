#ifndef CMDHANDLER_H
#define CMDHANDLER_H
#include "handler.h"
#include "searcheng.h"
#include "combiners.h"
#include <cstddef>

/**
 * Handles the QUIT command - COMPLETED in cmdhandler.cpp
 */
class QuitHandler : public Handler
{
public:
    QuitHandler();
    QuitHandler(Handler* next);

protected:

    virtual bool canHandle(const std::string& cmd) const;
    virtual HANDLER_STATUS_T process(SearchEng* eng, std::istream& instr, std::ostream& ostr);
};

/*********** OTHER CLASSES WILL NEED TO BE COMPLETED IN cmdhandler.cpp *******/
/***********  You will also need to declare the classes for handling   *******/
/***********          the AND, OR, and DIFF command handlers           *******/

/**
 * Handles the PRINT command
 */
class PrintHandler : public Handler
{
public:
    PrintHandler();
    PrintHandler(Handler* next);

protected:

    virtual bool canHandle(const std::string& cmd) const;
    virtual HANDLER_STATUS_T process(SearchEng* eng, std::istream& instr, std::ostream& ostr);
};

/**
 * Handles the INCOMING command
 */
class IncomingHandler : public Handler
{
public:
    IncomingHandler();
    IncomingHandler(Handler* next);

protected:

    virtual bool canHandle(const std::string& cmd) const;
    virtual HANDLER_STATUS_T process(SearchEng* eng, std::istream& instr, std::ostream& ostr);
};

/**
 * Handles the OUTGOING command
 */
class OutgoingHandler : public Handler
{
public:
    OutgoingHandler();
    OutgoingHandler(Handler* next);

protected:

    virtual bool canHandle(const std::string& cmd) const;
    virtual HANDLER_STATUS_T process(SearchEng* eng, std::istream& instr, std::ostream& ostr);
};

/****************************************************************************/
/* Declare the handler classes for AND, OR, and DIFF here                   */
/****************************************************************************/

/**
 * Handles the AND command
 */
class AndHandler : public Handler
{
public:
    AndHandler();
    AndHandler(Handler* next);

protected:

    virtual bool canHandle(const std::string& cmd) const;
    virtual HANDLER_STATUS_T process(SearchEng* eng, std::istream& instr, std::ostream& ostr);
};

/*Handles the OR command*/
class OrHandler : public Handler
{
public:
    OrHandler();
    OrHandler(Handler* next);

protected:

    virtual bool canHandle(const std::string& cmd) const;
    virtual HANDLER_STATUS_T process(SearchEng* eng, std::istream& instr, std::ostream& ostr);
};

/*Handles the Diff command*/
class DiffHandler : public Handler
{
public:
    DiffHandler();
    DiffHandler(Handler* next);

protected:

    virtual bool canHandle(const std::string& cmd) const;
    virtual HANDLER_STATUS_T process(SearchEng* eng, std::istream& instr, std::ostream& ostr);
};

class PRAndHandler : public Handler
{
public:
    PRAndHandler();
    PRAndHandler(Handler* next);

protected:

    virtual bool canHandle(const std::string& cmd) const;
    virtual HANDLER_STATUS_T process(SearchEng* eng, std::istream& instr, std::ostream& ostr);
};

class PROrHandler : public Handler
{
public:
    PROrHandler();
    PROrHandler(Handler* next);

protected:

    virtual bool canHandle(const std::string& cmd) const;
    virtual HANDLER_STATUS_T process(SearchEng* eng, std::istream& instr, std::ostream& ostr);
};

/**
 * Performs mergesort on the provided vector using
 * the Compare object, comp, to peform ALL comparisons
 *
 * Must run in O(n*log(n))
 */
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
        i++;
    }
    while (r < leftsize)
    {
        list[i] = right[r];
        r++;
        i++;
    }
}

template <class T, class Compare >
void mergeSort(std::vector<T>& list, Compare comp);

template<class T, class Compare>
void mergeSort(std::vector<T>& list, Compare comp)
{
    if (list.size() <= 1)
    {
        return;
    }
    size_t mid = list.size() / 2;
    std::vector<T> left;
    std::vector<T> right;
    for (size_t i = 0; i < mid; i++)
    {
        left.push_back(list[i]);
    }
    for (size_t j = mid; j < list.size(); j++)
    {
        right.push_back(list[j]);
    }
    mergeSort(left, comp);
    mergeSort(right, comp);
    mymergeSort(left, right, list, comp);
}
#endif
