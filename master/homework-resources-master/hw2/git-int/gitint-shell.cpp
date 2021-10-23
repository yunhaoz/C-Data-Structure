#include <iostream>
#include "gitint.h"
// Add any necessary headers
using namespace std;

void print_exception_message(const std::string& what_msg);

int main()
{

    GitInt g;
    string cmd_line;
    bool quit = false;
    const string PROMPT_STARTER = "$ ";

    g.print_menu();
    cout << PROMPT_STARTER;
    getline(cin, cmd_line);
    // Add your code here


    
    return 0;

}

void print_exception_message(const std::string& what_msg)
{
  cout << "Error - " << what_msg << endl;
}
