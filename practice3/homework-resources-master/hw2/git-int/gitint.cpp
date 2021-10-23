#include <iostream>
#include <sstream>
#include <algorithm>
#include <stdexcept>
#include "gitint.h"
#include <map>

using namespace std;

/*********************** Messages to use for errors ***************************/
const std::string INVALID_COMMAND = "Invalid command";
const std::string INVALID_OPTION = "Invalid option";
const std::string INVALID_COMMIT_NUMBER = "Invalid commit number";
const std::string LOG_COMMIT_STARTER = "Commit: ";



// Class implementation

GitInt:GitInt()
{
    
}

void GitInt::print_menu() const
{
    cout << "Menu:                          " << endl;
    cout << "===============================" << endl;
    cout << "create   filename int-value    " << endl;
    cout << "edit     filename int-value    " << endl;
    cout << "display  (filename)            " << endl;
    cout << "display  commit-num            " << endl;
    cout << "add      file1 (file2 ...)     " << endl;
    cout << "commit   \"log-message\"       " << endl;
    cout << "tag      (-a tag-name)         " << endl;
    cout << "log                            " << endl;
    cout << "checkout commit-num/tag-name   " << endl;
    cout << "diff                           " << endl;
    cout << "diff     commit                " << endl;
    cout << "diff     commit-n commit-m     " << endl;
}


bool GitInt::process_command(std::string cmd_line)
{
    bool quit = false;
    std::stringstream ss(cmd_line);
    std::string cmd;
    ss >> cmd;
    if (ss.fail()) throw std::runtime_error(INVALID_COMMAND);

    if (cmd == "quit") {
        quit = true;
    }
    // Continue checking/processing commands
    else if (cmd == "create")
    {
        string _filename;
        int _filevalue;
        ss >> _filename >> _filevalue;
        if (ss.fail()) throw std::runtime_error("Invalid input");
        create(_filename, _filevalue);
    }
    else if (cmd == "edit")
    {
        string _filename;
        int _filevalue;
        ss >> _filename >> _filevalue;
        if (ss.fail()) throw std::runtime_error("Invalid input");
        edit(_filename, _filevalue);
    }
    else if (cmd == "display")
    {
        string _input;
        ss >> _input;
        if (_input.size() == 0)
        {
            display_all();
        }
        else
        {
            display(_input);
        }
    }
    else if (cmd == "add")
    {
        string _input;
        getline(ss, _input);
        add(_input);
    }
    else if (cmd == "commit")
    {
        string _input;
        getline(ss, _input);
        //use find first of
        if (ss.fail()) throw std::runtime_error("Invalid message");
        size_t check1 = _input.find_first_of('"');
        size_t check2 = _input.find_first_of('"', check1 + 1);
        if (check1 + 1 == check2 || check1 == string::npos || check2 == string::npos)
        {
            throw std::runtime_error("Invalid message");
        }
        //ignore two sign
        commit(_input.substr(check1 + 1, check2));
    }
    else if (cmd == "tag")
    {
        string _input;
        getline(ss, _input);
        if (ss.fail())
        {
            tags();
        }
        else
        {
            create_tag(_input, head_);
        }
    }
    else if (cmd == "log")
    {
        log();
    }
    else if (cmd == "checkout")
    {
        int check;
        ss >> check;
        if (ss.fail())
        {
            ss.clear();
            string temp;
            ss >> temp;
            if(ss.fail())
                throw std::runtime_error("No checkout target");
            else
                checkout(temp);
        }
        else
        {
            checkout(check);
        }
    }
    else if (cmd == "diff")
    {
        int check1;
        ss >> check1;
        if (ss.fail())
        {
            diff(head_);
        }
        else
        {
            int check2;
            ss >> check2;
            if (ss.fail())
            {
                diff(check1);
            }
            else
            {
                diff(check1, check2);
            }
        }
    }
    return quit;
}

void GitInt::create(const std::string& filename, int value)
{
    map<string, int>::iterator it = myFile.find(filename);
    if (it != myFile.end())
    {
        throw std::invalid_argument("File already exists");
    }
    else
    {
        myFile.insert(make_pair(filename, value));
    }
}

void GitInt::edit(const std::string& filename, int value)
{
    map<string, int>::iterator it = myFile.find(filename);
    if (it == myFile.end())
    {
        throw std::invalid_argument("File doesn't exist");
    }
    else
    {
        myFile[filename] = value;
    }
}

void GitInt::display(const std::string& filename) const
{
    map<string, int>::iterator mt = myFile.find(filename);
    if (mt == myFile.end())
    {
        throw std::invalid_argument("File doesn't exist");
    }
    else
    {
        cout << filename << " : " << myFile[filename] << endl;
    }
}

void GitInt::display_all() const
{
    if (myFile.size() == 0)
        return;
    else
    {
        for (map<string, int>::iterator mt = myFile.begin(); mt != myFile.end(); ++mt)
        {
            cout << mt->first << " : " << mt->second << endl;
        }
    }
}

void GitInt::add(std::string filename)
{
    stringstream ss(filename);
    string fileName;
    ss >> fileName;
    while (!ss.fail())
    {
        map<string, int>::iterator it = myFile.find(fileName);
        if (it == myFile.end())
        {
            myStage.clear();
            throw std::invalid_argument("File doesn't exist");
        }
        else
        {
            myStage.insert(make_pair(fileName, myFile[fileName]));
        }
        ss >> fileName;
    }
}

void GitInt::commit(std::string message)
{
    if (message.size() == 0)
    {
        throw std::runtime_error("Invalid commit message");
    }
    else if (myStage.size() == 0)
    {
        throw std::runtime_error("NO files are staged");
    }
    else
    { 
        if (commits_.size() == 1)
        {
            CommitObj temp(message, myStage, 0);
            commits_.push_back(temp);
            head_++;
        }
        else
        {
            map<string, int>::iterator it;
            for (it = myStage.begin(); it != myStage.end(); ++it)
            {
                map<string, int>::iterator ct = commits_[head_].diffs_.find(it->first);
                if (ct != commits_[head_].diffs_.end())
                {
                    it->second = it->second - ct->second;
                }
            }
                CommitObj temp(message, myStage, head_);
                commits_.push_back(temp);
                head_ = commits_.size() - 1;
        }
    }
    
}

void GitInt::create_tag(const std::string& tagname, CommitIdx commit)
{
    stringstream ss(tagname);
    string signature;
    string tagName;
    ss >> signature >> tagName;
    if (ss.fail() || signature.compare("-a") != 0) throw std::runtime_error("Invalid tag name");
    else if (myTag.find(tagName) != myTag.end()) throw std::invalid_argument("Repeated tag name");
    else
    {
        myTag.insert(make_pair(tagname, commit));
        myTagVector.push_back(tagname);
    }
}

void GitInt::tags() const
{
    for (vector<string>::iterator it = myTagVector.begin(); it != myTagVector.end(); ++it)
    {
        cout << *it << endl;
    }
}

bool GitInt::checkout(CommitIdx commitIndex)
{
    stringstream ss(commitIndex);
    int tempInt;
    ss >> tempInt;
    if (ss.fail())
    {
        throw std::runtime_error("No checkout target");
    }
    if (commitIndex >= commits_.size())
    {
        throw std::invalid_argument("Invalid target");
    }
    myFile.clear();
    myFile = buildState(commitIndex, 0);
    head_ = commitIndex;
}

bool GitInt::checkout(std::string tag)
{
    stringstream ss(tag);
    string tagName;
    ss >> tagName;
    if (ss.fail())
    {
        throw std::runtime_error("No checkout target");
    }
    if (myTag.find(tagName) == myTag.end())
    {
        throw std::invalid_argument("Invalid target");
    }
    myFile.clear();
    myFile = buildState(myTag[tagName], 0);
    head_ = myTag[tagName];
}

void GitInt::log() const
{
    CommitIdx temp = head_;
    while (temp != 0)
    {
        cout << "Commit: " << temp << endl;
        cout << commits_[temp].msg_ << endl;
        temp = commits_[temp].parent_;
    }
}

void GitInt::diff(CommitIdx to) const
{
    if (valid_commit(to))
        throw std::invalid_argument("Invalid commit");
    map<string, int> temp = buildState(to, 0);
    for (map<string, int>::iterator it = myFile.begin(); it != myFile.end(); ++it)
    {
        if (temp.find(it->first) == temp.end())
        {
            cout << it->first << " : " << it->second << endl;
        }
        else if (it->second != temp[it->first])
        {
            cout << it->frist << " : " << (it->second - temp[it->first]) << endl;
        }
    }
}

void GitInt::diff(CommitIdx from, CommitIdx to) const
{
    if (valid_commit(from) || valid_commit(to))
    {
        throw std::invalid_argument("Invalid commit");
    }
    map<string, int> temp1 = buildState(from, 0);
    map<string, int> temp2 = buildState(to, 0);
    for (map<string, int>::iterator it = temp1.begin(); it != temp1.end(); ++it)
    {
        if (temp2.find(it->first) == temp2.end())
        {
            cout << it->first << " : " << it->second << endl;
        }
        else if (it->second != temp[it->first])
        {
            cout << it->first << " : " << (it->second - temp[it->first]) << endl;
        }
    }
}

bool GitInt::valid_commit(CommitIdx commit) const
{
    if (commit <= 0 || commit >= commits_.size())
    {
        return false;
    }
    else
    {
        return true;
    }
}

void GitInt::display_commit(CommitIdx commit) const
{
    if ( false == valid_commit(commit) ) {
        throw std::invalid_argument(INVALID_COMMIT_NUMBER);
    }
    display_helper(commits_[commit].diffs_);
}


std::map<std::string, int> GitInt::buildState(CommitIdx from, CommitIdx to = 0) const
{
    CommitIdx temp = from;
    map<string, int> accural;
    for (map<string, int>::iterator it = commits_[temp].diffs_.begin(); it != commits_[temp].diffs_.end(); ++it)
    {
        accural.insert(make_pair(it->first, it->second));
    }
    temp = commits_[from].parent_;
    while (temp != to)
    {
        for (map<string, int>::iterator it = accural.begin(); it != accural.end(); ++it)
        {
            if (commits_[temp].diffs_.find(it->first) != commits_[temp].diffs_.end())
            {
                it->first += commits_[temp].diffs_[it->first];
            }
        }
        temp = commits_[temp].parent_;
    }
    return accural;
}

void GitInt::display_helper(const std::map<std::string, int>& dat) const
{
    for (std::map<std::string, int>::const_iterator cit = dat.begin();
            cit != dat.end();
            ++cit) {
        std::cout << cit->first << " : " << cit->second << std::endl;
    }
}


void GitInt::log_helper(CommitIdx commit_num, const std::string& log_message) const
{
    std::cout << LOG_COMMIT_STARTER << commit_num << std::endl;
    std::cout << log_message << std::endl << std::endl;

}
