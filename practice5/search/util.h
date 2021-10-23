#ifndef UTIL_H
#define UTIL_H

#include <string>
#include <iostream>
#include <set>
#include "webpage.h"

/**
 * Returns an all lower-case version of the input string
 * [COMPLETED]
 *
 * @param[in] src
 *   String to convert
 * @return Lower-case version of src
 */
std::string conv_to_lower(std::string src);

/**
 * Returns an all upper-case version of the input string
 * [COMPLETED]
 *
 * @param[in] src
 *   String to convert
 * @return Upper-case version of src
 */
std::string conv_to_upper(std::string src);

/**
 * Prints results of a WebPageSet
 * [COMPLETED]
 *
 * @param[in] hits
 *   Set of WebPage pointers whose filenames should be displayed
 * @param[inout] ostr
 *   Stream to output results
 */
void display_hits(const WebPageSet& hits, std::ostream& ostr);

/**
 * Prints a vector of pairs of WebPage pointers & their rank
 * [COMPLETED]
 *
 * @param[in] hits
 *   Vector of pairs of WebPage pointers and each WebPage's corresponding pagerank score
 * @param[inout] ostr
 *   Stream to output results
 */
typedef std::vector<std::pair<WebPage*, double> > WebPageScorePairs;
void display_ranked_hits(WebPageScorePairs& hits, std::ostream& ostr);


/**
 * Extracts the extension from a filename
 * [COMPLETED]
 *
 * @param[in] hits
 *   Set of WebPage pointers whose filenames should be displayed
 * @param[inout] ostr
 *   Stream to output results
 */
std::string extract_extension(const std::string& filename);


#endif
