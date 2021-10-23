#include <map>
#include <set>
#include <vector>
#include <iostream>
#include <fstream>
#include "searcheng.h"


using namespace std;

std::string extract_extension(const std::string& filename);


SearchEng::SearchEng(PageParser* noExtensionParser)
{
    if (NULL == noExtensionParser)
    {
        throw std::invalid_argument("default parser cannot be NULL");
    }
    this->noExtensionParser_ = noExtensionParser;

    // Add additional code if necessary

}

SearchEng::~SearchEng()
{
    allPagesTerm.clear();
    allParsers.clear();
    allNamesWithPages.clear();
}


void SearchEng::register_parser(const std::string& extension, PageParser* parser)
{
    allParsers.insert(make_pair(extension, parser));
}

void SearchEng::read_pages_from_index(const std::string& index_file)
{
    ifstream ifile(index_file.c_str());
    if(ifile.fail()) {
        cerr << "Unable to open index file: " << index_file << endl;
    }

    // Parse all the files
    string filename;
    while(ifile >> filename) {
#ifdef DEBUG
        cout << "Reading " << filename << endl;
#endif
        read_page(filename);
    }
    ifile.close();
}

WebPage* SearchEng::retrieve_page(const std::string& page_name) const
{
    if (allNamesWithPages.find(page_name) == allNamesWithPages.end())
    {
        return NULL;
    }
    else
    {
        return allNamesWithPages[page_name];
    }
}

void SearchEng::display_page(std::ostream& ostr, const std::string& page_name) const
{
    if (allNamesWithPages.find(page_name) == allNamesWithPages.end())
    {
        throw std::invalid_argument("Nonexist pagename");
    }
    else
    {
        if (allParsers.find(extract_extension(page_name)) == allParsers.end())
        {
            throw std::logic_error;
        }
        else
        {
            ostr << allParsers.at(extract_extension(page_name))->display_text(page_name);
        }
    }
}

WebPageSet SearchEng::search(const std::vector<std::string>& terms, WebPageSetCombiner* combiner) const
{
    WebPageSet temp;
    WebPageSet temp2;
    if (allPagesTerm.find(terms[0]) != allPagesTerm.end())
    {
        temp = allPagesTerm[terms[0]];
    }
    //for(int i=0;i<terms.size();i++)
    for (int i=terms.size()-1;i>0;i++)
    {
        if (allPagesTerm.find(terms[i]) == allPagesTerm.end())
        {
            temp2 = WebPageSet();
        }
        else
        {
            temp2 = allPagesTerm[terms[i]];
        }
        temp = combiner->combine(temp, temp2);
    }
    return temp;
}

void SearchEng::read_page(const std::string& filename)
{
    string extension = extract_extension(filename);
    //if we don't have this page
    if (allNamesWithPages.find(filename) == allNamesWithPages.end())
    {
        allNamesWithPages.insert(make_pair(filename, new WebPage(filename)));
    }
    if (allParsers.find(extension) != allParsers.end())
    {
        StringSet allTerms;
        StringSet allLinks;
        allParsers.at(extension)->parse(filename, allTerms, allLinks);
        allNamesWithPages[filename]->all_terms(allTerms);
        for (StringSet::iterator it = allLinks.begin(); it != allLinks.end(); ++it)
        {
            //insert a new one if the page doesn't exist
            if (allNamesWithPages.find(*it) == allNamesWithPages.end())
            {
                allNamesWithPages.insert(make_pair(*it, new WebPage(*it)));
            }
            //inlink
            allNamesWithPages[*it]->add_incoming_link(allNamesWithPages.at(filename));
            //outlink
            allNamesWithPages[filename]->add_outgoing_link(allNamesWithPages.at(*it));
        }

        for (StringSet::iterator it = allTerms.begin(); it != allTerms.end(); ++it)
        {
            if (allPagesTerm.find(conv_to_lower(*it)) == allPagesTerm.end())
            {
                WebPageSet temp;
                allPagesTerm.insert(make_pair(conv_to_lower(*it), temp));
            }
            allPagesTerm[conv_to_lower(*it)].insert(allNamesWithPages.at(filename));
        }
    }
}

std::string extract_extension(const std::string& filename)
{
    size_t idx = filename.rfind(".");
    if (idx == std::string::npos) {
        return std::string();
    }
    return filename.substr(idx + 1);
}


