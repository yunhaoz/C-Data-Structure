#ifndef WPSCOMBINER_H
#define WPSCOMBINER_H

#include "webpage.h"

class WebPageSetCombiner
{
public:
    /**
     * Destructor
     */
    virtual ~WebPageSetCombiner() {}

    /**
     * Combines two sets of webpages into a resulting set based on some strategy
     *
     * @param[in] setA
     *   first set of webpages
     * @param[in] setB
     *   second set of webpages
     * @return set of webpages that results from the combination strategy
     */
    virtual
    WebPageSet combine(const WebPageSet& setA, const WebPageSet& setB) = 0;
};


#endif
