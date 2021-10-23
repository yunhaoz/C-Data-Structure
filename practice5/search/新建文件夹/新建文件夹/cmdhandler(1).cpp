#include "cmdhandler.h"
#include "util.h"
#include "combiners.h"
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
IncomingHandler::IncomingHandler(){

}
IncomingHandler::IncomingHandler(Handler* next)
    : Handler(next)
{

}
bool IncomingHandler::canHandle(const std::string& cmd) const{
    return cmd == "INCOMING";
}
Handler::HANDLER_STATUS_T IncomingHandler::process(SearchEng* eng, std::istream& instr, std::ostream& ostr){
    string name;
    if(!(instr >> name)) return HANDLER_ERROR;//return error if cannot read the name
    WebPage* page = eng->retrieve_page(name);
    if(page==nullptr) return HANDLER_ERROR;
    display_hits(page->incoming_links(),ostr);
    return HANDLER_OK;
}
OutgoingHandler::OutgoingHandler(){

}
OutgoingHandler::OutgoingHandler(Handler* next)
    : Handler(next)
{

}
bool OutgoingHandler::canHandle(const std::string& cmd) const{
    return cmd == "OUTGOING";
}
Handler::HANDLER_STATUS_T OutgoingHandler::process(SearchEng* eng, std::istream& instr, std::ostream& ostr){
    string name;
    if(!(instr >> name)) return HANDLER_ERROR;//return error if cannot read the name
    WebPage* page = eng->retrieve_page(name);
    if(page==nullptr) return HANDLER_ERROR;
    display_hits(page->outgoing_links(),ostr);
    return HANDLER_OK;
}
AndHandler::AndHandler(){

}
AndHandler::AndHandler(Handler* next)
    : Handler(next)
{

}
bool AndHandler::canHandle(const std::string& cmd) const{
    return cmd == "AND";
}
Handler::HANDLER_STATUS_T AndHandler::process(SearchEng* eng, std::istream& instr, std::ostream& ostr){
    //construct the vectors of terms
    vector<string> v;
    string name;
    while(instr>>name){
        v.push_back(conv_to_lower(name));
    }
    ANDWebPageSetCombiner add = ANDWebPageSetCombiner();
    display_hits(eng->search(v,&add),ostr);
    return HANDLER_OK;
}
OrHandler::OrHandler(){

}
OrHandler::OrHandler(Handler* next)
    : Handler(next)
{

}
bool OrHandler::canHandle(const std::string& cmd) const{
    return cmd == "OR";
}
Handler::HANDLER_STATUS_T OrHandler::process(SearchEng* eng, std::istream& instr, std::ostream& ostr){
    //construct the vectors of terms
    vector<string> v;
    string name;
    while(instr>>name){
        v.push_back(conv_to_lower(name));
    }
    ORWebPageSetCombiner orc = ORWebPageSetCombiner();
    display_hits(eng->search(v,&orc),ostr);
    return HANDLER_OK;
}
DiffHandler::DiffHandler(){

}
DiffHandler::DiffHandler(Handler* next)
    : Handler(next)
{

}
bool DiffHandler::canHandle(const std::string& cmd) const{
    return cmd == "DIFF";
}
Handler::HANDLER_STATUS_T DiffHandler::process(SearchEng* eng, std::istream& instr, std::ostream& ostr){
    //construct the vectors of terms
    vector<string> v;
    string name;
    while(instr>>name){
        v.push_back(conv_to_lower(name));
    }
    DiffWebPageSetCombiner diff = DiffWebPageSetCombiner();
    display_hits(eng->search(v,&diff),ostr);
    return HANDLER_OK;
}
PrandHandler::PrandHandler(){

}
PrandHandler::PrandHandler(Handler* next)
        : Handler(next)
{

}
bool PrandHandler::canHandle(const std::string& cmd) const{
    return cmd == "PRAND";
}
Handler::HANDLER_STATUS_T PrandHandler::process(SearchEng* eng, std::istream& instr, std::ostream& ostr){
    //construct the vectors of terms
    vector<string> v;
    string name;
    bool is_r;
    instr>>name;
    if(instr.fail()) return HANDLER_ERROR;
    if(name=="r") is_r=true;
    else if(name=="n") is_r = false;
    else return HANDLER_ERROR;
    r r1; n n1;
    while(instr>>name){
        v.push_back(conv_to_lower(name));
    }
    ANDWebPageSetCombiner add = ANDWebPageSetCombiner();
    WebPageSet w = eng->search(v,&add);
    WebPageSet temp = w;
    //expand the web page set
    for(WebPageSet::iterator it = w.begin();it!=w.end();++it){
        WebPage* web_ptr  = *it;
        for(WebPageSet::iterator in_it = web_ptr->incoming_links().begin();in_it!=web_ptr->incoming_links().end();++in_it){
            temp.insert(*in_it);
        }
        for(WebPageSet::iterator out_it = web_ptr->outgoing_links().begin();out_it!=web_ptr->outgoing_links().end();++out_it){
            temp.insert(*out_it);
        }
    }
    WebPageScorePairs scorePairs = eng->pageRank(temp);
    if(is_r) mergeSort(scorePairs,r1);
    else mergeSort(scorePairs,n1);
    display_ranked_hits(scorePairs,ostr);
    return HANDLER_OK;
}
ProrHandler::ProrHandler(){

}
ProrHandler::ProrHandler(Handler* next)
        : Handler(next)
{

}
bool ProrHandler::canHandle(const std::string& cmd) const{
    return cmd == "PROR";
}
Handler::HANDLER_STATUS_T ProrHandler::process(SearchEng* eng, std::istream& instr, std::ostream& ostr){
    //construct the vectors of terms
    vector<string> v;
    string name;
    bool is_r;
    instr>>name;
    if(instr.fail()) return HANDLER_ERROR;
    if(name=="r") is_r=true;
    else if(name=="n") is_r = false;
    else return HANDLER_ERROR;
    r r1; n n1;
    while(instr>>name){
        v.push_back(conv_to_lower(name));
    }
    ORWebPageSetCombiner orc = ORWebPageSetCombiner();
    WebPageSet w = eng->search(v,&orc);
    WebPageSet temp = w;
//    display_hits(w,ostr);
    for(WebPageSet::iterator it = w.begin();it!=w.end();++it){
        WebPage* web_ptr  = *it;
        for(WebPageSet::iterator in_it = web_ptr->incoming_links().begin();in_it!=web_ptr->incoming_links().end();++in_it){
            temp.insert(*in_it);
        }
        for(WebPageSet::iterator out_it = web_ptr->outgoing_links().begin();out_it!=web_ptr->outgoing_links().end();++out_it){
            temp.insert(*out_it);
        }
    }
//    cout << "-------------" << endl;
//    display_hits(temp,ostr);
    WebPageScorePairs scorePairs = eng->pageRank(temp);
    if(is_r) mergeSort(scorePairs,r1);
    else mergeSort(scorePairs,n1);
    display_ranked_hits(scorePairs,ostr);
    return HANDLER_OK;
}
