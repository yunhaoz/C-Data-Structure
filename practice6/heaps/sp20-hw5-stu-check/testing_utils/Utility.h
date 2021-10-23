#ifndef UTIL_H
#define UTIL_H

#include <set>
#include <vector>
#include <iostream>

// Used from http://stackoverflow.com/questions/216823/whats-the-best-way-to-trim-stdstring
// trim from start
std::string &ltrim(std::string &s) ;

// trim from end
std::string &rtrim(std::string &s) ;

// trim from both ends
std::string &trim(std::string &s) ;

void RemoveWhiteSpace(std::string &s);

void RemoveEmptyLines(std::istream& is, std::ostream& os);

bool Approximately(double a, double b, double eps = 0.01);

template <typename T>
bool OrderedComparison(const std::vector<T>& lhs, const std::vector<T>& rhs)
{
	if (lhs.size() != rhs.size()) return false;

	for(size_t i = 0; i < lhs.size(); ++i)
	{
		if (!(lhs[i] == rhs[i])) return false;
	}
	return true;
}

template <typename T>
bool UnOrderedComparison(const std::set<T>& lhs, const std::set<T>& rhs)
{
	return IsSubsetOf(lhs, rhs) && IsSubsetOf(rhs, lhs);
}

template <typename T>
bool IsSubsetOf(const std::set<T>& subset, const std::set<T>& full)
{
	for (typename std::set<T>::iterator it = subset.begin(); it != subset.end(); ++it)
	{
		if (full.find(*it) == full.end()) return false;
	}
	return true;
}

// used when we are in a checker function and need to call another checker function.
// If the given function returns a false AssertionResult, this returns it from the current function.
#define SUB_ASSERT(functioncall) \
{ \
	testing::AssertionResult result = functioncall;\
	if(!result) \
	{\
		return result; \
	} \
}

#endif