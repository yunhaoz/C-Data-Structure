#include <iostream>
#include <deque>
#include <limits>
#include "hgraph.h"
// You may add other necessary #includes other than <algorithm>

using namespace std;


HollywoodGraph::HollywoodGraph(int n, vector<string> names)
{
	aMatrix = vector<vector<bool>>(n, vector<bool>(n, false));
	for (int j = 0; j < n; j++)
	{
		nameMap.insert(make_pair(names[j], j));
	}
}

HollywoodGraph::~HollywoodGraph()
{

}

bool HollywoodGraph::addConnection(string person1, string person2)
{
	int p1 = nameMap[person1];
	int p2 = nameMap[person2];
	if (aMatrix[p1][p2] == true)
	{
		return false;
	}
	else
	{
		aMatrix[p1][p2] = true;
	}
}

int HollywoodGraph::countConnections(string person1)
{
	int p = nameMap[person1];
	int sum = 0;
	for (int i = 0; i < aMatrix[p].size(); i++)
	{
		if (aMatrix[p][i] == true)
		{
			sum++;
		}
	}
	return sum;
}

bool HollywoodGraph::degreesOfSeparation(string person1, string person2, int k, int& calls)
{
	int p1 = nameMap[person1];
	int p2 = nameMap[person2];

}

