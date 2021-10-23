#include "expr-reader.h"
#include <stdexcept>
#include <sstream>
#include <iostream>
using namespace std;

Node* exprReaderHelper(std::istream& istr);

/**
 * @pre istr pointer is just after the '('
 */
Node* exprReader(std::istream& istr)
{
    string token;
    istr >> token;
    if( token != "(") {
        cout << "Expected ( to start expression" << endl;
        return NULL;
    }
    return exprReaderHelper(istr);
}

Node* exprReaderHelper(std::istream& istr)
{
    Node* mynode = new Node;
    int idx = 0;
    string token;
    istr >> token;
#ifdef DEBUG
    cout << "New sub-expression, starting with token " << token << endl;
#endif
    while( istr && token != ")") {
        if(idx == 1) {
            const char ops[] = { '+', '-', '*', '/' };
            const unsigned NUMOPS = 4;
            bool validOp = false;
            for(unsigned i=0; i < NUMOPS; i++) {
                if(token[0] == ops[i]) validOp = true;
            }
            if(!validOp) {
                stringstream serr;
                serr << "Invalid operation token: " << token << endl;
                throw std::runtime_error(serr.str());
            }
#ifdef DEBUG
            cout << mynode << " setting op/val to " << token << endl;
#endif
            mynode->val = token;
        }
        else if(idx > 1 && idx % 2 == 1) {
            if(token != mynode->val) {
                stringstream serr;
                serr << "Non-matching operation token: " << token << ", expected: " << mynode->val << " " << " mynode=" << mynode <<  endl;
                throw std::runtime_error(serr.str());
            }
            // otherwise we can just proceed/discard
        }
        else if(token[0] == '(') {
            if(token != "(") {
                throw std::runtime_error("( requires space after it");
            }
            mynode->children.push_back( exprReaderHelper(istr) );
        }
        else {
            Node* myleaf = new Node;
            myleaf->val = token;
            mynode->children.push_back(myleaf);
        }
        idx++;
        istr >> token;
#ifdef DEBUG
        cout << "Reading next token,index = " << token << "," << idx << endl;
#endif
    }
    if( mynode->children.empty()) {
        stringstream serr;
        serr << "Parentheses can't be empty" << endl;
        throw std::runtime_error(serr.str());
    }
    else if( mynode->val != "" && mynode->children.size() < 2 ) {
        stringstream serr;
        serr << "Binary operator requires 2 children" << endl;
        throw std::runtime_error(serr.str());
    }

    return mynode;
}
