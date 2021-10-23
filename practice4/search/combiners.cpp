#include "searcheng.h"

// Place your function implementations for your derived
// WebPageSetCombiner classes here

WebPageSet WebPageSetAnd::combine(const WebPageSet& setA, const WebPageSet& setB)
{
    WebPageSet temp;
    for (WebPageSet::iterator it = setA.begin(); it != setA.end(); ++it)
    {
        if (setB.find(*it) != setB.end())
        {
            temp.insert(*it);
        }
    }
    return temp;
}

WebPageSet WebPageSetOr::combine(const WebPageSet& setA, const WebPageSet& setB)
{
    WebPageSet temp;
    for (WebPageSet::iterator it = setA.begin(); it != setA.end(); ++it)
    {
        temp.insert(*it);
    }
    for (WebPageSet::iterator it = setB.begin(); it != setB.end(); ++it)
    {
        temp.insert(*it);
    }
    return temp;
}

WebPageSet WebPageSetDiff::combine(const WebPageSet& setA, const WebPageSet& setB)
{
    WebPageSet temp;
    for (WebPageSet::iterator it = setA.begin(); it != setA.end(); ++it)
    {
        if (setB.find(*it) == setB.end())
        {
            temp.insert(*it);
        }
    }
    return temp;
}