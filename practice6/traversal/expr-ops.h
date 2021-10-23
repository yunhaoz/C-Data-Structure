#ifndef EXPR_OPS_H
#define EXPR_OPS_H
#include "expr-reader.h"
#include <ostream>

/**
 *  Outputs the expression in a valid format for the given ostream.
 *  This must be done in such a way that the outputted expression
 *  is valid such that it can be read back in by the provided
 *  expression reader in a subsequent execution.
 *
 *  @param[inout] ostr
 *    Output stream to which to write the expression
 *  @param[in] n
 *    Pointer to the root node of the expression to print.
 *    May be NULL in which case no output should be generated.
 *
 */
void exprPrinter(std::ostream& ostr, Node* n);
void printHelper(std::ostream& ostr, Node* n, string parent);


/**
 *  Solves the expression given by the root node of the expression
 *  tree and returns its value.
 *
 *  @pre The expression must be valid, with leaf nodes containing
 *       integers.
 *
 *  @param[in] n
 *    Pointer to the root node of the expression to print.
 *    May be NULL in which case no output should be generated.
 *  @return Integer solution to the expression
 *
 *  @throws std::logic_error
 *    Throws if the argument, n, is null.
 *
 */
int exprSolver(Node* n);

/**
 *  Determines if the expression tree rooted at n is valid,
 *  which is defined as all leaves storing strings that are
 *  convertible to integers.
 *
 *  @param[in] n
 *    Pointer to the root node of the expression to test.
 *
 *  @returns true if the expression is valid or n is NULL
 *
 */
bool exprIsValid(Node* n);

#endif
