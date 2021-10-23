//
// Students: write your trie implementation below.
//

#include "trie.h"

//----------------------------------------------
// DO NOT MODIFY THE CONSTRUCTOR OR DESTRUCTOR.
//----------------------------------------------

SparseTrie::SparseTrie()
{
    // DO NOT MODIFY.
    // The root node is always present and represents an
    // empty string. It should never be removed until
    // the destructor runs.
    root = new TNode('\0', false);
}

SparseTrie::~SparseTrie()
{
    // LEAVE BLANK. Don't worry about deallocation.
}

void SparseTrie::insert(std::string key)
{
    TNode* node = root;
    TNode* prev = node;
    size_t length = 0;
    node = node->children;
    while (length != key.length() - 1)
    {
        if (node == nullptr)
        {
            prev->children = new TNode(key[length], false);
            length++;
            prev = prev->children;
            node = prev->children;
        }
        //not match
        else if (node->letter != key[length])
        {
            if (node->next == nullptr)
            {
                node->children = new TNode(key[length], false);
                length++;
                prev = node;
                node = node->children;
            }
            else
            {
                prev = node;
                node = node->next;
            }
        }
        //match
        else if (node->letter == key[length])
        {
            prev = node;
            node = node->children;
        }
    }
    if (node != nullptr)
    {
        node->terminal = true;
    }
    else
    {
        prev->children = new TNode(key[length], true);
    }
}

bool SparseTrie::contains(std::string key)
{
    size_t length = 0;
    TNode* node = root;
    node = node->children;
    while (node != nullptr && length != key.length() - 1)
    {
        if (node->letter == key[length])
        {
            node = node->children;
            length++;
        }
        else
        {
            node = node->next;
        }
    }
    if (length == key.length() - 1)
    {
        return node->terminal;
    }
    else
    {
        return false;
    }
}

