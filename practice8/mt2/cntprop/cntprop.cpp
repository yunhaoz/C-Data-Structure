// For this problem you may put ALL your code in this one file
// (implementation of countProperty(), helpers, and main()
// [though read the comments above main() as we have some template
//  code that should not be changed so we can test your code.]
#include <iostream>
#include <cstdlib>
using namespace std;

// You may NOT change this struct definition

struct Node {
    int key;
    Node *left, *right, *parent;
    // Constructor for convenience
    Node(int k, Node* lt = NULL, Node* rt = NULL, Node* p = NULL) :
        key(k), left(lt), right(rt), parent(p)
    {}
};

// You may NOT change this function prototype
int countProperty(Node * root);
int checkChild(Node* temp);

// You may add any prototypes of helper functions here
void helper(Node* root, int sum);

// Now implement the countProperty function and any necessary helpers
int countProperty(Node * root)
{
    int sum = 0;

}

void helper(Node* root, int& sum)
{
    if (root->left = NULL || root->right == NULL)
    {
        return;
    }
    if (checkChild(root->left) == 0 && checkChild(root->right) == 2)
    {
        sum++;
    }
    else if (checkChild(root->left) == 2 && checkChild(root->right) == 0) {
        sum++;
    }
    helper(root->left, sum);
    helper(root->right, sum);
}

int checkChild(Node* temp)
{
    int count = 0;
    if (temp->left != NULL)
    {
        count++;
    }
    if (temp->right != NULL)
    {
        count++;
    }
    return count;
}

// Do not change this
#ifndef CS104_TEST

// You may enter a main test function here if desired,
// or you may leave it blank. Do not change the lines below
// the end of main().
int main( )
{


    return 0;
}


// Do not change this
#else
// this is the testing file we will use for grading...ignore this
#include "cntprop-test.cpp"
#endif
