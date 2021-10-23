#ifndef EXPR_READER_H
#define EXPR_READER_H
#include <istream>
#include <vector>
#include <string>

/**
 * Node representing an (sub)expression, defined as
 *
 * op        := '+' | '-' | '*' | '/'
 * literal   := string
 * expr      := literal |
 *              '(' expr ')' |
 *              expr op expr
 * root-expr := '(' expr ')'
 *
 * val field contains the string literal or the operator
 */
struct Node {
    std::string val;
    std::vector<Node*> children;
};

/**
 * Reads an expression whose format is described in the problem description.
 *
 * @pre istr get pointer shall be located just after a '(' was read
 *
 * @param [in] istr
 *   Input stream from which to read the expression
 * @return Pointer to the root of dynamically allocated tree (nodes)
 *
 * @throws std::runtime_error
 *   If there are syntax errors in the input expression
 */
Node* exprReader(std::istream& istr);

#endif
