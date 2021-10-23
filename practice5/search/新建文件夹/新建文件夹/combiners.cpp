#include "searcheng.h"
#include "combiners.h"

// Place your function implementations for your derived
// WebPageSetCombiner classes here

/*the andcombiner*/
WebPageSet ANDWebPageSetCombiner::combine(const WebPageSet& setA, const WebPageSet& setB)
{
    WebPageSet temp;
    /*check whether the term in setA also in setB*/
    for (WebPageSet::iterator it = setA.begin(); it != setA.end(); ++it)
    {
        if (setB.find(*it) != setB.end())
        {
            /*if it is then insert*/
            temp.insert(*it);
        }
    }
    return temp;
}

WebPageSet ORWebPageSetCombiner::combine(const WebPageSet& setA, const WebPageSet& setB)
{
    WebPageSet temp;
    /*insert all the term of setA*/
    for (WebPageSet::iterator it = setA.begin(); it != setA.end(); ++it)
    {
        temp.insert(*it);
    }
    /*insert all the term of setB*/
    for (WebPageSet::iterator it = setB.begin(); it != setB.end(); ++it)
    {
        temp.insert(*it);
    }
    return temp;
}

WebPageSet DiffWebPageSetCombiner::combine(const WebPageSet& setA, const WebPageSet& setB)
{
    WebPageSet temp;
    /*check whether the term in setA also in setB*/
    for (WebPageSet::iterator it = setA.begin(); it != setA.end(); ++it)
    {
        /*if it is not then insert*/
        if (setB.find(*it) == setB.end())
        {
            temp.insert(*it);
        }
    }
    return temp;
}