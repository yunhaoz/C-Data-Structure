#include <iostream>
#include <fstream>
#include <set>
#include <map>
#include <vector>
#include "avlbst.h"

using namespace std;


typedef std::map<char, int> ANSWER_MAP_T;
void printSolution(ANSWER_MAP_T& answer, std::ostream& ostr);
bool search(ANSWER_MAP_T& answer, std::map<char, std::set<char>>& bordCountry, int country, char curr);

int main(int argc, char* argv[])
{
    if(argc < 3) {
        cout << "Please supply a map input file and output filename" << endl;
        return 1;
    }
    // COMPLETE main()
    std::ifstream myfile(argv[1]);
    int country, row, col;
    char temp;
    // use it to store a graph representation, 
    // the key being the country and the value being a list of its neighbors(piazza)
    std::map<char, std::set<char>> bordCountry;
    myfile >> country >> row >> col;
    ANSWER_MAP_T answer;
    /*char countryList[row][col];*/
    std::vector<std::vector<char>> countryList(row, std::vector<char>(col, '\0'));
    //initialize the countryList and bordCountry
    for (int i = 0; i < row; i++)
    {
        for (int j = 0; j < col; j++)
        {
            myfile >> temp;
            countryList[i][j] = temp;
            std::set<char> tempset;
            bordCountry.insert(make_pair(temp, tempset));
        }
    }

    //generate the map
    for (int i = 0; i < row; i++)
    {
        for (int j = 0; j < col; j++)
        {
            char curr = countryList[i][j];
            //compare left
            if ((i - 1) >= 0)
            {
                char check = countryList[i - 1][j];
                if (curr != check)
                {
                    bordCountry[curr].insert(check);
                    bordCountry[check].insert(curr);
                }
            }
            //compare right
            if ((i + 1) < row)
            {
                char check = countryList[i + 1][j];
                if (curr != check)
                {
                    bordCountry[curr].insert(check);
                    bordCountry[check].insert(curr);
                }
            }
            //compare below
            if ((j + 1) < col)
            {
                char check = countryList[i][j + 1];
                if (curr != check)
                {
                    bordCountry[curr].insert(check);
                    bordCountry[check].insert(curr);
                }
            }
        }
    }

    bool result = search(answer, bordCountry, country, 'A');

    // Call printSolution to output the answers to the output file
    std::ofstream output(argv[2]);
    printSolution(answer, output);
    myfile.close();
    output.close();
    return 0;
}

bool search(ANSWER_MAP_T& answer, std::map<char, std::set<char>>& bordCountry, int country, char curr)
{
    //check where to stop
    if (answer.size() == country)
    {
        return true;
    }
    if (answer.find(curr) == answer.end())
    {
        //try all the color
        for (int i = 0; i <= 4; i++)
        {
            //check whether can insert this color
            bool cancolor = true;
            for (set<char>::iterator it = bordCountry.at(curr).begin(); it != bordCountry.at(curr).end(); ++it)
            {
                //check 
                if (answer.find(*it) != answer.end() && answer[*it] == i)
                {
                    cancolor = false;
                }
            }
            if (cancolor)
            {
                answer.insert(make_pair(curr, i));
                if (search(answer, bordCountry, country, curr + 1))
                {
                    return true;
                }
                answer.erase(curr);
            }
        }
    }
    else
    {
        if (search(answer, bordCountry, country, curr + 1))
        {
            return true;
        }
    }
    return false;
}



void printSolution(ANSWER_MAP_T& answer, std::ostream& os)
{
    for(ANSWER_MAP_T::iterator it = answer.begin();
            it!= answer.end();
            ++it) {
        os << it->first << " " << it->second << endl;
    }
}

