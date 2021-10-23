#include "expr-reader.h"
#include "expr-ops.h"
#include <iostream>
#include <fstream>
#include <string>
#include <stdexcept>
using namespace std;

int main(int argc, char* argv[])
{
    if(argc < 2) {
        cout << "Please provide an output file to print the expression" << endl;
        return 1;
    }
    try {
        Node* root = exprReader(std::cin);

        if(NULL == root) {
            cout << "Reader returned NULL" << endl;
            return 1;
        }


        if( exprIsValid(root) ) {
            cout << "Valid" << endl;
            std::ofstream ofile(argv[1]);
            exprPrinter(ofile, root);
            ofile.close();
            cout << "Result: " << exprSolver(root) << endl;
        }
        else {
            cout << "Invalid" << endl;
        }
    }
    catch(std::exception& e) {
        cerr << e.what() << endl;
        return 1;
    }
    return 0;
}
