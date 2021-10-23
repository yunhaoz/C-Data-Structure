#ifndef TRIE_H
#define TRIE_H
//-----------------------------------------------------------------------------
//  YOU MAY ONLY ADD PRIVATE HELPER FUNCTIONS TO THIS FILE.
//   - PROVIDE YOUR IMPLEMENTATION AND HELPER FUNCTIONS IN trie.cpp
//-----------------------------------------------------------------------------

#include <string>

struct TNode {
    char letter;   // next letter in the key
                   //  Indicates which child this node corresponds to
                   //  from its parent
    bool terminal; // true if this node is a terminal node
    TNode* children, *next; // head pointer to children and sibling linked lists

    // Constructor for convenience
    TNode(char myletter, bool isTerminalNode=false, TNode* mynext=nullptr) :
        letter(myletter), terminal(isTerminalNode), children(nullptr), next(mynext)
    { }

};

class SparseTrie
{

public:
    SparseTrie(); // Constructor - Provided. Leave as is.
    ~SparseTrie(); // Destructor - Provided. Leave as is.

    // Adds a new key to the trie.
    // If the key already exists, the trie remains in the same state.
    // Note: The empty string ("") is a legal key that may be inserted

    void insert(std::string key);

    // Returns true if a key has been inserted into the trie previously
    // and false, otherwise.
    bool contains(std::string key);

private:
    // Add private helper function prototypes here
    

    // This should be the ONLY data member. See the constructor.
    // You may NOT add data members or alter this data member.
    // NOTE: The root node's next pointer should always be nullptr.
    // The root node's children pointer is not null iff any non-empty
    // strings have been inserted to the trie.
    TNode* root;

public:

    // Gets the root node.  Used for testing/grading. Do not change.
    TNode * getRoot() { return root; }
};

#endif
