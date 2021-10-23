#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <set>
#include "md_parser.h"
#include "txt_parser.h"
#include "util.h"

using namespace std;

int main(int argc, char* argv[])
{
    if (argc < 3) {
        cout << "Must provide an index file and output file" << endl;
        return 1;
    }

    // ---------------------- [TO BE COMPLETED] -------------------------------
    std::map<std::string, PageParser*> parsers;
    parsers.insert(make_pair("md", new MDParser));
    parsers.insert(make_pair("txt", new TXTParser));
    std::set<std::string> processed;
    std::ofstream outfile;
    std::ifstream infile;
    outfile.open(argv[2]);
    PageParser a;
    a.crawl(parsers,infile,processed, outfile);
    outfile.close();
    // You may add cleanup code here if necessary

    return 0;
}
