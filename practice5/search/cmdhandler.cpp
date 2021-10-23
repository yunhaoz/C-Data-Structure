#include "cmdhandler.h"
#include "util.h"


using namespace std;

QuitHandler::QuitHandler()
{

}

QuitHandler::QuitHandler(Handler* next)
    : Handler(next)
{

}

bool QuitHandler::canHandle(const std::string& cmd) const
{
    return cmd == "QUIT";

}

Handler::HANDLER_STATUS_T QuitHandler::process(SearchEng* eng, std::istream& instr, std::ostream& ostr)
{
    return HANDLER_QUIT;
}

PrintHandler::PrintHandler()
{

}

PrintHandler::PrintHandler(Handler* next)
    : Handler(next)
{

}

bool PrintHandler::canHandle(const std::string& cmd) const
{
    return cmd == "PRINT";

}

Handler::HANDLER_STATUS_T PrintHandler::process(SearchEng* eng, std::istream& instr, std::ostream& ostr)
{
    string name;

    if (!(instr >> name)) {
        return HANDLER_ERROR;
    }
    try {
        eng->display_page(ostr, name);
    }
    catch (std::exception& e) {
        return HANDLER_ERROR;
    }
    return HANDLER_OK;
}

IncomingHandler::IncomingHandler()
{
}

IncomingHandler::IncomingHandler(Handler* next)
    :Handler(next)
{
}

bool IncomingHandler::canHandle(const std::string& cmd) const
{
    return cmd == "INCOMING";
}
/*the incoming handler*/
Handler::HANDLER_STATUS_T IncomingHandler::process(SearchEng* eng, std::istream& instr, std::ostream& ostr)
{
    string name;
    /*check the input*/
    if (!(instr >> name)) {
        return HANDLER_ERROR;
    }
    try {
        /*Webpagetemp in order to iterating output the filename*/
        WebPageSet temp = eng->retrieve_page(name)->incoming_links();
        ostr << temp.size() << endl;
        for (WebPageSet::iterator it = temp.begin(); it != temp.end(); ++it)
        {
            ostr << (*it)->filename() << endl;
        }
    }
    catch (std::exception & e) {
        return HANDLER_ERROR;
    }
    return HANDLER_OK;
}

OutgoingHandler::OutgoingHandler()
{
}

OutgoingHandler::OutgoingHandler(Handler* next)
    :Handler(next)
{
}

bool OutgoingHandler::canHandle(const std::string& cmd) const
{
    return cmd=="OUTGOING";
}

Handler::HANDLER_STATUS_T OutgoingHandler::process(SearchEng* eng, std::istream& instr, std::ostream& ostr)
{
    string name;
/*check the input*/
    if (!(instr >> name)) {
        return HANDLER_ERROR;
    }
    try {
        /*like the incoming function*/
        WebPageSet temp = eng->retrieve_page(name)->outgoing_links();
        ostr << temp.size() << endl;
        for (WebPageSet::iterator it = temp.begin(); it != temp.end(); ++it)
        {
            ostr << (*it)->filename() << endl;
        }
    }
    catch (std::exception & e) {
        return HANDLER_ERROR;
    }
    return HANDLER_OK;
}

AndHandler::AndHandler()
{
}

AndHandler::AndHandler(Handler* next)
    :Handler(next)
{
}

bool AndHandler::canHandle(const std::string& cmd) const
{
    return cmd == "AND";
}

Handler::HANDLER_STATUS_T AndHandler::process(SearchEng* eng, std::istream& instr, std::ostream& ostr)
{
    vector<std::string> temp;
    string tempterm;
    /*input the term*/
    while (instr >> tempterm)
    {
        /*tranlate into the lower form*/
        temp.push_back(conv_to_lower(tempterm));
        if(instr.fail())
            break;
    }
    WebPageSetAnd tempCom = WebPageSetAnd();
    /*use the andcombiner to process*/
    WebPageSet addset = eng->search(temp, &tempCom);
    
    display_hits(addset,ostr);
    
    return HANDLER_OK;
}

OrHandler::OrHandler()
{
}

OrHandler::OrHandler(Handler* next)
    :Handler(next)
{
}

bool OrHandler::canHandle(const std::string& cmd) const
{
    return cmd=="OR";
}

Handler::HANDLER_STATUS_T OrHandler::process(SearchEng* eng, std::istream& instr, std::ostream& ostr)
{
    vector<std::string> temp;
    string tempterm;
    /*just like the andhandler bur use the or combiner instead*/
    while (instr >> tempterm)
    {
        temp.push_back(conv_to_lower(tempterm));
        if(instr.fail())
            break;
    }
    WebPageSetOr tempCom = WebPageSetOr();
    WebPageSet addset = eng->search(temp,&tempCom);
    
    display_hits(addset,ostr);
    
    return HANDLER_OK;
}

DiffHandler::DiffHandler()
{
}

DiffHandler::DiffHandler(Handler* next)
    :Handler(next)
{
}

bool DiffHandler::canHandle(const std::string& cmd) const
{
    return cmd=="DIFF";
}

Handler::HANDLER_STATUS_T DiffHandler::process(SearchEng* eng, std::istream& instr, std::ostream& ostr)
{
    vector<std::string> temp;
    string tempterm;
    /*just like the andhandler bur use the diff combiner instead*/
    while (instr >> tempterm)
    {
        temp.push_back(conv_to_lower(tempterm));
        if(instr.fail())
            break;
    }
    WebPageSetDiff tempCom = WebPageSetDiff();
    WebPageSet addset = eng->search(temp, &tempCom);
    display_hits(addset,ostr);
    
    return HANDLER_OK;
}


struct compN
{
    bool operator()(const pair<WebPage*,double>& lhs, const pair<WebPage*, double>& rhs)
    {
        return ((lhs.first)->filename() < (rhs.first)->filename())
    }
};

struct compR
{
    bool operator()(const pair<WebPage*, double>& lhs, const pair<WebPage*, double>& rhs)
    {
        return lhs.second > rhs.second;
    }
};


PRAndHandler::PRAndHandler()
{
}

PRAndHandler::PRAndHandler(Handler* next)
    :Handler(next)
{
}

bool PRAndHandler::canHandle(const std::string& cmd) const
{
    return cmd == "PRAND";
}

HANDLER_STATUS_T PRAndHandler::process(SearchEng* eng, std::istream& instr, std::ostream& ostr)
{
    vector<std::string> temp;
    string tempterm;
    /*input the term*/
    char check;
    instr >> check;
    if (check != 'r' || check != 'n')
    {
        return HANDLER_ERROR;
    }
    while (instr >> tempterm)
    {
        /*tranlate into the lower form*/
        temp.push_back(conv_to_lower(tempterm));
        if (instr.fail())
            break;
    }
    WebPageSetCombiner tempCom = ANDWebPageSetCombiner();
    /*use the andcombiner to process*/
    WebPageSet addset = eng->search(temp, &tempCom);
    WebPageScorePairs result = SearchEng::pageRank(addset);
    if (check == 'r')
    {
        mergeSort(result, compR);
    }
    else
    {
        mergeSort(result, compN);
    }

    display_ranked_hits(result, ostr);

    return HANDLER_OK;
}

PROrHandler::PROrHandler()
{
}

PROrHandler::PROrHandler(Handler* next)
    :Handler(next)
{
}

bool PROrHandler::canHandle(const std::string& cmd) const
{
    return cmd=PROR;
}

HANDLER_STATUS_T PROrHandler::process(SearchEng* eng, std::istream& instr, std::ostream& ostr)
{
    vector<std::string> temp;
    string tempterm;
    /*input the term*/
    char check;
    instr >> check;
    if (check != 'r' && check != 'n')
    {
        return HANDLER_ERROR;
    }
    while (instr >> tempterm)
    {
        /*tranlate into the lower form*/
        temp.push_back(conv_to_lower(tempterm));
        if (instr.fail())
            break;
    }
    ORWebPageSetCombiner tempCom = ORWebPageSetCombiner();
    /*use the andcombiner to process*/
    WebPageSet addset = eng->search(temp, &tempCom);


    //expand the candidate set
    WebPageSet expandaddset = addset;
    for (WebPageSet::iterator it = addset.begin(); it != addset.end(); it++)
    {
        WebPageSet income = (*it)->incoming_links();
        for (WebPageSet::iterator init = income.begin(); init != income.end(); init++)
        {
            expandaddset.insert(*init);
        }
        WebPageSet outcome = (*it)->outgoing_links();
        for (WebPageSet::iterator outit = outcome.begin(); outit != outcome.end(); outit++)
        {
            expandaddset.insert(*outit);
        }
    }

    WebPageScorePairs result = eng->pageRank(expandaddset);
    if (check == 'r')
    {
        compR comp;
        mergeSort(result, comp);
    }
    else
    {
        compN comp;
        mergeSort(result, comp);
    }

    display_ranked_hits(result, ostr);

    return HANDLER_OK;
}
