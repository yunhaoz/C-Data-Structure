#include "expr-reader.h"
#include "expr-ops.h"
#include <iostream>

using namespace std;
/*-----------------------------------------------------------------------------
 *  Implement the following functions below.
 *  Processing of each (sub)expression must be recursive (i.e. loops
 *    may only be used to iterate through the child vector of the given node).
 *  You may define recursive helper functions to adapt the function signatures.
 *    However, only exprPrinter likely needs a helper function.
 ----------------------------------------------------------------------------*/

void exprPrinter(std::ostream& ostr, Node* n)
{
    if (n == NULL)
    {
        return;
    }
    printHelper(ostr, n, NULL);
    ostr << endl;
}

void printHelper(std::ostream& ostr, Node* n, string parent)
{
    if (n->children.size()==0)
    {
        ostr << n->val;
        return;
    }
    ostr << "( ";
    for (vector<Node*>::iterator it = n->children.begin(); it != n->children.end(); ++it)
    {
        printHelper(ostr, *it, (*it)->val);
        ostr << parent << " ";
    }
    ostr << ") ";
}


int exprSolver(Node* n)
{
    /* Should never be called on NULL expression -- Do Not Modify */
    if(n == NULL ) {
        throw std::logic_error("Can't solve a NULL expression");
    }
    if (n->children.size() == 0)
    {
        return std::stoi(n->val);
    }
    //it should be inorder
    int temp = n->children.at(0);
    for (int i = 1; i < (n->children).size(); i++)
    {
        if (n->val == '+')
        {
            temp += exprSolver(*it);
        }
        else if (n->val == '-')
        {
            temp -= exprSolver(*it);
        }
        else if (n->val == '*')
        {
            temp *= exprSolver(*it);
        }
        else if (n->val == '/')
        {
            temp /= exprSolver(*it);
        }
    }
    return temp;
}


bool exprIsValid(Node* n)
{    
    if (n == NULL)
    {
        return true;
    }
    //check the leaf node
    if ((n->children).size() == 0)
    {
        int strsize = (n->val).size();
        if (strsize == 0) return false;
        //check the if string size is 1
        else if (strsize == 1)
        {
            if (n->val.at(0) == '-' || n->val.at(0) < '0' || n->val.at(0) > '9') return false;
        }
        //if stringsize > 1
        else
        {
            //check the first char
            if (n->val.at(0) != '-')
            {
                if (n->val.at(0) < '0' || n->val.at(0) > '9')
                {
                    return false;
                }
            }
            for (int j = 1; j < strsize; j++)
            {
                if (n->val.at(j) < '0' || n->val.at(j) > '9')
                {
                    return false;
                }
            }
        }
        return true;
    }
    bool status = true;
    for (vector<Node*>::iterator it = n->children.begin(); it != n->children.end(); ++it)
    {
        bool temp = exprIsValid(*it);
        if (temp == false) status = false;
    }
    return status;
}
