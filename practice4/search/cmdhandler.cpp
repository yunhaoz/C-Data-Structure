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

Handler::HANDLER_STATUS_T IncomingHandler::process(SearchEng* eng, std::istream& instr, std::ostream& ostr)
{
    string name;

    if (!(instr >> name)) {
        return HANDLER_ERROR;
    }
    try {
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

    if (!(instr >> name)) {
        return HANDLER_ERROR;
    }
    try {
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
    while (temp >> tempterm)
    {
        temp.push_back(conv_to_lower(tempterm));
    }
    WebPageSet addset = eng->search(temp, WebPageSetAnd);
    try {
        ostr << addset.size() << endl;
        for (WebPageSet::iterator it = addset.begin(); it != addset.end(); ++it)
        {
            ostr << (*it)->filename() << endl;
        }
    }
    catch (std::exception & e) {
        return HANDLER_ERROR;
    }
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

HANDLER_STATUS_T OrHandler::process(SearchEng* eng, std::istream& instr, std::ostream& ostr)
{
    vector<std::string> temp;
    string tempterm;
    while (temp >> tempterm)
    {
        temp.push_back(conv_to_lower(tempterm));
    }
    WebPageSet addset = eng->search(temp, WebPageSetOr);
    try {
        ostr << addset.size() << endl;
        for (WebPageSet::iterator it = addset.begin(); it != addset.end(); ++it)
        {
            ostr << (*it)->filename() << endl;
        }
    }
    catch (std::exception & e) {
        return HANDLER_ERROR;
    }
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

HANDLER_STATUS_T DiffHandler::process(SearchEng* eng, std::istream& instr, std::ostream& ostr)
{
    vector<std::string> temp;
    string tempterm;
    while (temp >> tempterm)
    {
        temp.push_back(conv_to_lower(tempterm));
    }
    WebPageSet addset = eng->search(temp, WebPageSetDiff);
    try {
        ostr << addset.size() << endl;
        for (WebPageSet::iterator it = addset.begin(); it != addset.end(); ++it)
        {
            ostr << (*it)->filename() << endl;
        }
    }
    catch (std::exception & e) {
        return HANDLER_ERROR;
    }
    return HANDLER_OK;
}
