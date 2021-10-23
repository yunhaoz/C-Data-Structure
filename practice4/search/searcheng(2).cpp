#include <map>
#include <set>
#include <vector>
#include <iostream>
#include <fstream>
#include "searcheng.h"
#include "util.h"

using namespace std;

//std::string extract_extension(const std::string& filename)
//{
//    size_t idx = filename.rfind(".");
//    if (idx == std::string::npos) {
//        return std::string();
//    }
//    return filename.substr(idx + 1);
//}

SearchEng::SearchEng(PageParser* noExtensionParser)
{
    if (NULL == noExtensionParser)
    {
        throw std::invalid_argument("default parser cannot be NULL");
    }
    this->noExtensionParser_ = noExtensionParser;

    // Add additional code if necessary
}
// don't forget destructor

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
        try
        {
            read_page(filename);
        }
        catch (std::exception& e) {
            cout << e.what() << endl;
        }
    }
    ifile.close();
}

SearchEng::~SearchEng() {
    delete noExtensionParser_;
    for(map<string, WebPage*>::iterator it=webPageMapWithName.begin();it!=webPageMapWithName.end();it++)
    {
        delete (it->second);
    }
    webPageMapWithName.clear();
    for(map<string,PageParser*>::iterator it=pageParserMap_.begin();it!=pageParserMap_.end();it++)
    {
        delete (it->second);
    }
    pageParserMap_.clear();

}

void SearchEng::register_parser(const std::string &extension, PageParser *parser) {
    // see if I can insert duplicate element
    pageParserMap_.insert(make_pair(extension,parser));
}

void SearchEng::display_page(std::ostream &ostr, const std::string &page_name) const {
    ifstream ifile(page_name);
    if(ifile.fail()){
        cout<<"can't open "<<page_name<<endl;
        throw invalid_argument("Page name doesn't exist");
    }
    string extension = extract_extension(page_name);
    if(!extension.empty())
    {
        if(pageParserMap_.find(extension)!=pageParserMap_.end())
        {
            ostr<<pageParserMap_.at(extension)->display_text(page_name);
        } else{
            throw logic_error("no appropriate parser");
            // cerr<<"Extension invalid, no appropriate parser available"<<endl;
        }
    }
    else
    {
        ostr<<noExtensionParser_->display_text(page_name);
    }
}


WebPageSet SearchEng::search(const std::vector<std::string> &terms, WebPageSetCombiner *combiner) const {
    //logn + mlogm
    //mlogm is go through all webPages and find if contains
    WebPageSet tempA, tempEnd;
    if(terms.empty()) return tempA;
    if(webPageSetMapWithTerm.find(terms[0])!=webPageSetMapWithTerm.end())
    {
        tempA=webPageSetMapWithTerm.at(terms[0]);
    }
    for(int j=terms.size()-1;j>0;j--)
    {
        if(webPageSetMapWithTerm.find(terms[j])!=webPageSetMapWithTerm.end())
        {
            tempEnd = webPageSetMapWithTerm.at(terms[j]);
        }
        else
        {
            tempEnd = WebPageSet();
        }
        tempA = combiner->combine(tempA,tempEnd);
    }
    return tempA;
}

WebPage *SearchEng::retrieve_page(const std::string &page_name) const {
    return webPageMapWithName.at(page_name);
}

void SearchEng::read_page(const std::string &filename) {
    string extension = extract_extension(filename);
    if(webPageMapWithName.find(filename)==webPageMapWithName.end()) //if not existing
        webPageMapWithName.insert(make_pair(filename, new WebPage)); // insert the new webpage
    // if able to parse
    if (pageParserMap_.find(extension) != pageParserMap_.end()) {
        StringSet tempTerms;
        StringSet tempLinks;
        // use the appropriate parser to parse through the page
        pageParserMap_.at(extension)->parse(filename,tempTerms,tempLinks);
        // set all the searchable terms
        webPageMapWithName[filename]->all_terms(tempTerms);

        // loop through all links in the current file
        for(StringSet::const_iterator cit=tempLinks.begin();cit!=tempLinks.end();cit++)
        {
            // if the outgoing link doesn't exist, add it to searchEng's private map
            if(webPageMapWithName.find(*cit)==webPageMapWithName.end()) //if the webpage does not exist
            {
                webPageMapWithName.insert(make_pair(*cit, new WebPage));
//                webPageMapWithName[*cit]->add_incoming_link(webPageMapWithName[*cit]);
                webPageMapWithName[*cit]->filename(*cit);
            }
            // add incoming link to the other side
            webPageMapWithName[*cit]->add_incoming_link(webPageMapWithName.at(filename));

            // add outgoing link to this webPage
            webPageMapWithName[filename]->add_outgoing_link(webPageMapWithName.at(*cit));
            // logic error may occur here
        }
//        webPageMapWithName[filename]->add_incoming_link(webPageMapWithName[filename]);
        webPageMapWithName[filename]->filename(filename);

        for(StringSet::const_iterator cit=tempTerms.begin();cit!=tempTerms.end();cit++)
        {
            if(webPageSetMapWithTerm.find(conv_to_lower(*cit))==webPageSetMapWithTerm.end())
            {
                WebPageSet dummy;
                webPageSetMapWithTerm.insert(make_pair(conv_to_lower(*cit),dummy));
            }
            webPageSetMapWithTerm[conv_to_lower(*cit)].insert(webPageMapWithName.at(filename));

        }

    }
    else {
        throw logic_error("no appropriate parser");
        // cerr<<"Extension invalid, no appropriate parser available"<<endl;
    }
}




