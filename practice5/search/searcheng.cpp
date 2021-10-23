#include <map>
#include <set>
#include <vector>
#include <iostream>
#include <fstream>
#include "searcheng.h"
#include "util.h"


using namespace std;




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
    for(map<std::string, WebPage*>::iterator it =allNamesWithPages.begin();it!=allNamesWithPages.end();it++)
    {
        delete it->second;
    }
}

/*relate all the extension with the parser*/
void SearchEng::register_parser(const std::string& extension, PageParser* parser)
{
    allParsers.insert(make_pair(extension, parser));
}
/*read and parse the page*/
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
        return allNamesWithPages.at(page_name);
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
        /*find the extension of the page*/
        if (allParsers.find(extract_extension(page_name)) == allParsers.end())
        {
            throw std::logic_error("Don't have right extension");
        }
        else
        {
            /*use the display_text matching the extension of the page*/
            ostr << allParsers.at(extract_extension(page_name))->display_text(page_name);
        }
    }
}

WebPageSet SearchEng::search(const std::vector<std::string>& terms, WebPageSetCombiner* combiner) const
{
    WebPageSet temp;
    WebPageSet temp2;
    /*find the term*/
    if (allPagesTerm.find(terms[0]) != allPagesTerm.end())
    {
        temp = allPagesTerm.at(terms[0]);
    }
    //for(int i=0;i<terms.size();i++)
    for (int i=terms.size()-1;i>0;i--)
    {
        if (allPagesTerm.find(terms[i]) == allPagesTerm.end())
        {
            /**/
            temp2 = WebPageSet();
        }
        else
        {
            temp2 = allPagesTerm.at(terms[i]);
        }
        /*use the combiner*/
        temp = combiner->combine(temp, temp2);
    }
    return temp;
}

std::vector<std::pair<WebPage*, double>> SearchEng::pageRank(const WebPageSet& in_pages)
{
    std::map<WebPage*, double> update;
    std::map<WebPage*, double> final;
    std::vector<std::pair<WebPage*, double>> mypair;
    
    //initialize the orihinal case 1/n
    for (WebPageSet::iterator it = in_pages.begin(); it != in_pages.end(); it++)
    {
        final.insert(make_pair(*it, (1.0 / in_pages.size())));
    }
    //loop for 20 times
    for (int i = 0; i < 20; i++)
    {
        /*double result = 0;*/
        //every inpage
        for (WebPageSet::iterator inpageit = in_pages.begin(); inpageit != in_pages.end(); inpageit++)
        {
            //put all the incoming link
            WebPageSet incominglink = (*inpageit)->incoming_links;
            double result = 0;//for every page calculate their own result
            for (WebPageSet::iterator inlinkit = incominglink.begin(); inlinkit != incominglink.end(); inlinkit++)
            {
                //it is in the candidate set
                if (in_pages.find(*inlinkit) != in_pages.end())
                {
                    WebPageSet outgoinglink = (*inlinkit)->outgoing_links;
                    int nodes = 0;
                    for (WebPageSet::iterator outlinkit = outgoinglink.begin(); outlinkit != outgoinglink.end(); outlinkit++)
                    {
                        //if it is in the candidate set
                        if (in_pages.find(*outlinkit) != in_pages.end())
                        {
                            nodes++;
                        }
                    }
                    //add the self-loop if necessary
                    if (outgoinglink.find(*inlinkit) == outgoinglink.end())
                    {
                        nodes++;
                    }
                    //for each incominglink calculate the result
                    //sum of p(u,t)/d+(u)
                    result += final[*inlinkit] / nodes;
                }
            }
            //add the self for the current if necessary
            if (incominglink.find(*inpageit) == incominglink.end())
            {
                WebPageSet outgoinglink = (*inpageit)->outgoing_links;
                int nodes = 0;
                for (WebPageSet::iterator outlinkit = outgoinglink.begin(); outlinkit != outgoinglink.end(); outlinkit++)
                {
                    //if it is in the candidate set
                    if (in_pages.find(*outlinkit) != in_pages.end())
                    {
                        nodes++;
                    }
                }
                //add the self-loop if necessary
                if (outgoinglink.find(*inpageit) == outgoinglink.end())
                {
                    nodes++;
                }
                //for each incominglink calculate the result
                //sum of p(u,t)/d+(u)
                result += final[*inpageit] / nodes;
            }
            //need to be store in the update, or the result will chang during the process
            update[*inpageit] = result * (1 - 0.15) + 0.15 / (in_pages.size());
        }
        //update the final
        std::map<WebPage*, double>::iterator mt;
        for (mt = update.begin(); mt != update.end(); mt++)
        {
            final[mt->first] = mt->second;
        }
    }
    //copy to the vector
    for (std::map<WebPage*, double>::iterator vit = final.begin(); vit != final.end(); vit++)
    {
        //push back the pair
        mypair.push_back(*vit);
    }
    return mypair;
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
        /*parse the page and put all the term and outlink*/
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
            /*insert the page who have the term*/
            if (allPagesTerm.find(conv_to_lower(*it)) == allPagesTerm.end())
            {
                WebPageSet temp;
                allPagesTerm.insert(make_pair(conv_to_lower(*it), temp));
            }
            allPagesTerm[conv_to_lower(*it)].insert(allNamesWithPages.at(filename));
        }
    }
}



