#ifndef RBBST_H
#define RBBST_H

#include <iostream>
#include <exception>
#include <cstdlib>
#include <algorithm>
#include "bst.h"

struct KeyError { };

/**
* A special kind of node for an AVL tree, which adds the balance as a data member, plus
* other additional helper functions. You do NOT need to implement any functionality or
* add additional data members or helper functions.
*/
template <typename Key, typename Value>
class AVLNode : public Node<Key, Value>
{
public:
    // Constructor/destructor.
    AVLNode(const Key& key, const Value& value, AVLNode<Key, Value>* parent);
    virtual ~AVLNode();

    // Getter/setter for the node's height.
    char getBalance () const;
    void setBalance (char balance);
    void updateBalance(char diff);

    // Getters for parent, left, and right. These need to be redefined since they
    // return pointers to AVLNodes - not plain Nodes. See the Node class in bst.h
    // for more information.
    virtual AVLNode<Key, Value>* getParent() const override;
    virtual AVLNode<Key, Value>* getLeft() const override;
    virtual AVLNode<Key, Value>* getRight() const override;

protected:
    char balance_;
};

/*
  -------------------------------------------------
  Begin implementations for the AVLNode class.
  -------------------------------------------------
*/

/**
* An explicit constructor to initialize the elements by calling the base class constructor and setting
* the color to red since every new node will be red when it is first inserted.
*/
template<class Key, class Value>
AVLNode<Key, Value>::AVLNode(const Key& key, const Value& value, AVLNode<Key, Value> *parent) :
    Node<Key, Value>(key, value, parent), balance_(0)
{

}

/**
* A destructor which does nothing.
*/
template<class Key, class Value>
AVLNode<Key, Value>::~AVLNode()
{

}

/**
* A getter for the balance of a AVLNode.
*/
template<class Key, class Value>
char AVLNode<Key, Value>::getBalance() const
{
    return balance_;
}

/**
* A setter for the balance of a AVLNode.
*/
template<class Key, class Value>
void AVLNode<Key, Value>::setBalance(char balance)
{
    balance_ = balance;
}

/**
* Adds diff to the balance of a AVLNode.
*/
template<class Key, class Value>
void AVLNode<Key, Value>::updateBalance(char diff)
{
    balance_ += diff;
}

/**
* An overridden function for getting the parent since a static_cast is necessary to make sure
* that our node is a AVLNode.
*/
template<class Key, class Value>
AVLNode<Key, Value> *AVLNode<Key, Value>::getParent() const
{
    return static_cast<AVLNode<Key, Value>*>(this->parent_);
}

/**
* Overridden for the same reasons as above.
*/
template<class Key, class Value>
AVLNode<Key, Value> *AVLNode<Key, Value>::getLeft() const
{
    return static_cast<AVLNode<Key, Value>*>(this->left_);
}

/**
* Overridden for the same reasons as above.
*/
template<class Key, class Value>
AVLNode<Key, Value> *AVLNode<Key, Value>::getRight() const
{
    return static_cast<AVLNode<Key, Value>*>(this->right_);
}


/*
  -----------------------------------------------
  End implementations for the AVLNode class.
  -----------------------------------------------
*/


template <class Key, class Value>
class AVLTree : public BinarySearchTree<Key, Value>
{
public:
    virtual void insert (const std::pair<const Key, Value> &new_item); // TODO
    virtual void remove(const Key& key);  // TODO
protected:
    virtual void nodeSwap( AVLNode<Key,Value>* n1, AVLNode<Key,Value>* n2);

    // Add helper functions here
    virtual void insertfix(AVLNode<Key, Value>* parent, AVLNode<Key, Value>* temp);
    virtual void rotateLeft(AVLNode<Key, Value>* temp);
    virtual void rotateRight(AVLNode<Key, Value>* temp);
    virtual void changeBalance(AVLNode<Key, Value>* parent, AVLNode<Key, Value>* temp);
    virtual void removefix(AVLNode<Key, Value>* temp, char diff);
};

/*
 * Recall: If key is already in the tree, you should 
 * overwrite the current value with the updated value.
 */
template<class Key, class Value>
void AVLTree<Key, Value>::insert (const std::pair<const Key, Value> &new_item)
{
    // TODO
    if (this->empty())
    {
        AVLNode<Key, Value>* temp = new AVLNode<Key, Value>(new_item.first, new_item.second, NULL);
        temp->setBalance(0);
        /*BinarySearchTree<Key, Value>::root_ = temp;*/
        this->root_ = temp;
        return;
    }
    AVLNode<Key, Value>* temp = static_cast<AVLNode<Key, Value>*>(this->root_);
    AVLNode<Key, Value>* parent;
    bool isleft;
    while (temp != NULL)
    {
        if (new_item.first == temp->getKey())
        {
            temp->setValue(new_item.second);
            return;
        }
        else if (new_item.first < temp->getKey())
        {
            parent = temp;
            temp = temp->getLeft();
            isleft = true;
        }
        else if (new_item.first > temp->getKey())
        {
            parent = temp;
            temp = temp->getRight();
            isleft = false;
        }
    }
    AVLNode<Key, Value>* a = new AVLNode<Key, Value>(new_item.first, new_item.second, parent);
    if (isleft)
    {
        parent->setLeft(a);
        if (parent->getBalance() == 1)
        {
            parent->setBalance(0);
            return;
        }
        else
        {
            parent->setBalance(-1);
            insertfix(parent->getParent(), parent);
        }
    }
    else
    {
        parent->setRight(a);
        if (parent->getBalance() == -1)
        {
            parent->setBalance(0);
            return;
        }
        else
        {
            parent->setBalance(1);
            insertfix(parent->getParent(), parent);
        }
    }
    return;
}

template<class Key, class Value>
void AVLTree<Key, Value>::insertfix(AVLNode<Key, Value>* parent, AVLNode<Key, Value>* temp)
{
    if (temp == NULL || parent == NULL)
    {
        return;
    }
    else
    {
        changeBalance(parent, temp);
        if (parent->getBalance() == 0)
        {
            return;
        }
        else if (parent->getBalance() == 1 || parent->getBalance() == -1)
        {
            insertfix(parent->getParent(), parent);
        }
        else if (parent->getBalance() == 2 || parent->getBalance() == -2)//three case
        {
            if (parent->getBalance() == -2 && parent->getLeft()->getBalance() == -1)
            {
                parent->setBalance(0);
                temp->setBalance(0);
                rotateRight(parent);
            }
            else if (parent->getBalance() == 2 && parent->getRight()->getBalance() == 1)
            {
                parent->setBalance(0);
                temp->setBalance(0);
                rotateLeft(parent);
            }
            else if (parent->getBalance() == -2 && parent->getLeft()->getBalance() == 1)
            {
                //three cases
                if (temp->getRight()->getBalance() == -1)
                {
                    temp->setBalance(0);
                    parent->setBalance(1);
                    temp->getRight()->setBalance(0);
                }
                else if (temp->getRight()->getBalance() == 0)
                {
                    temp->setBalance(0);
                    parent->setBalance(0);
                    temp->getRight()->setBalance(0);
                }
                else if (temp->getRight()->getBalance() == 1)
                {
                    temp->setBalance(-1);
                    parent->setBalance(0);
                    temp->getRight()->setBalance(0);
                }
                rotateLeft(temp);
                rotateRight(parent);
            }
            else if (parent->getBalance() == 2 && parent->getRight()->getBalance() == -1)
            {
                //three cases
                if (temp->getLeft()->getBalance() == -1)
                {
                    temp->setBalance(1);
                    parent->setBalance(0);
                    temp->getLeft()->setBalance(0);
                }
                else if (temp->getLeft()->getBalance() == 0)
                {
                    temp->setBalance(0);
                    parent->setBalance(0);
                    temp->getLeft()->setBalance(0);
                }
                else if (temp->getLeft()->getBalance() == 1)
                {
                    temp->setBalance(0);
                    parent->setBalance(-1);
                    temp->getLeft()->setBalance(0);
                }
                rotateRight(temp);
                rotateLeft(parent);
            }
            return;
        }
    }
}

template<class Key, class Value>
void AVLTree<Key, Value>::changeBalance(AVLNode<Key, Value>* parent, AVLNode<Key, Value>* temp)
{
    //if is the root
    if (parent == NULL || temp == NULL)
    {
        return;
    }
    //left
    if (parent->getLeft() == temp)
    {
        parent->updateBalance(-1);
    }
    //right
    else
    {
        parent->updateBalance(1);
    }
}

template<class Key, class Value>
void AVLTree<Key, Value>::rotateRight(AVLNode<Key, Value>* temp)
{
    //set the parent, the new parent and the changing node
    AVLNode<Key, Value>* parent = temp->getParent();
    AVLNode<Key, Value>* child = temp->getLeft();
    AVLNode<Key, Value>* change = temp->getLeft()->getRight();
    if (change != NULL)
    {
        change->setParent(temp);
    }
    if (parent == NULL)
    {
        //if temp is root, then the child will be the new root
        this->root_ = child;
    }
    else
    {
        //is left child
        if (parent->getLeft() == temp)
        {
            parent->setLeft(child);
        }
        else
        {
            parent->setRight(child);
        }
    }
    //changing the temp's parent and the changing nodes
    child->setParent(parent);
    child->setRight(temp);
    temp->setParent(child);
    temp->setLeft(change);
}

template<class Key, class Value>
void AVLTree<Key, Value>::rotateLeft(AVLNode<Key, Value>* temp)
{
    //set the parent, the new parent and the changing node
    AVLNode<Key, Value>* parent = temp->getParent();
    AVLNode<Key, Value>* child = temp->getRight();
    AVLNode<Key, Value>* change = temp->getRight()->getLeft();
    if (change != NULL)
    {
        change->setParent(temp);
    }
    if (parent == NULL)
    {
        //if temp is root, then the child will be the new root
        this->root_ = child;
    }
    else
    {
        //is left child
        if (parent->getLeft() == temp)
        {
            parent->setLeft(child);
        }
        else//right child
        {
            parent->setRight(child);
        }
    }
    child->setParent(parent);
    child->setLeft(temp);
    temp->setParent(child);
    temp->setRight(change);
}


/*
 * Recall: The writeup specifies that if a node has 2 children you
 * should swap with the predecessor and then remove.
 */
template<class Key, class Value>
void AVLTree<Key, Value>:: remove(const Key& key)
{
    // TODO
    //inherited members must be done by scoping (i.e. BinarySearchTree<Key,Value>:: or preceding with the keyword this->
    if (this->empty())
    {
        return;
    }
    Node<Key, Value>* find = this->internalFind(key);
    if (find == NULL)
    {
        return;
    }
    //cast to avlnode
    AVLNode<Key, Value>* temp = static_cast<AVLNode<Key, Value>*>(find);
    //If the node to be removed has two children, swap with its predecessor
    if (temp->getLeft() != NULL && temp->getRight() != NULL)
    {
        /*AVLNode<Key, Value>* pred = this->predecessor(temp);*/
        //must cast
        AVLNode<Key, Value>* pred = static_cast<AVLNode<Key, Value>*>(this->predecessor(temp));
        nodeSwap(temp, pred);
    }
    AVLNode<Key, Value>* parent = temp->getParent();
    char diff;
    if (parent == NULL)
    {
        diff = 0;
    }
    else
    {
        if (parent->getLeft() == temp)//left child
        {
            diff = 1;
        }
        else//right child
        {
            diff = -1;
        }
    }
    this->removeHelper(temp);
    removefix(parent, diff);
}

template<class Key, class Value>
void AVLTree<Key, Value>::removefix(AVLNode<Key, Value>* temp, char diff)
{
    if (temp == NULL)
    {
        return;
    }
    AVLNode<Key, Value>* parent = temp->getParent();
    char ndiff;
    if (parent == NULL)
    {
        ndiff = 0;
    }
    else
    {
        if (parent->getLeft() == temp)//left child
        {
            ndiff = 1;
        }
        else//right child
        {
            ndiff = -1;
        }
    }
    //diff = -1
    if (diff == -1)
    {
        if ((temp->getBalance() + diff) == -2)
        {
            //three cases
            if (temp->getLeft()->getBalance() == -1)
            {
                rotateRight(temp);
                temp->setBalance(0);
                temp->getLeft()->setBalance(0);
                removefix(parent, ndiff);
            }
            else if (temp->getLeft()->getBalance() == 0)
            {
                rotateRight(temp);
                temp->setBalance(-1);
                temp->getLeft()->setBalance(1);
                return;
            }
            else if (temp->getLeft()->getBalance() == 1)
            {
                //three cases
                AVLNode<Key, Value>* rchild = temp->getLeft()->getRight();
                rotateLeft(temp->getLeft());
                rotateRight(temp);
                if (rchild->getBalance() == 1)
                {
                    temp->setBalance(0);
                    temp->getLeft()->setBalance(-1);
                    rchild->setBalance(0);
                }
                else if (rchild->getBalance() == 0)
                {
                    temp->setBalance(0);
                    temp->getLeft()->setBalance(0);
                    rchild->setBalance(0);
                }
                else if (rchild->getBalance() == -1)
                {
                    temp->setBalance(1);
                    temp->getLeft()->setBalance(0);
                    rchild->setBalance(0);
                }
                removefix(parent, ndiff);
            }
        }
        else if ((temp->getBalance() + diff) == -1)
        {
            temp->setBalance(-1);
            return;
        }
        else if ((temp->getBalance() + diff) == 0)
        {
            temp->setBalance(0);
            removefix(parent, ndiff);
        }
    }
    else if (diff == 1)
    {
        //similar to before but mirror
        if ((temp->getBalance() + diff) == 2)
        {
            //three cases
            if (temp->getRight()->getBalance() == 1)
            {
                rotateLeft(temp);
                temp->setBalance(0);
                temp->getRight()->setBalance(0);
                removefix(parent, ndiff);
            }
            else if (temp->getRight()->getBalance() == 0)
            {
                rotateLeft(temp);
                temp->setBalance(1);
                temp->getRight()->setBalance(-1);
                return;
            }
            else if (temp->getRight()->getBalance() == -1)
            {
                //three cases
                AVLNode<Key, Value>* lchild = temp->getRight()->getLeft();
                rotateRight(temp->getRight());
                rotateLeft(temp);
                if (lchild->getBalance() == -1)
                {
                    temp->setBalance(0);
                    temp->getRight()->setBalance(1);
                    lchild->setBalance(0);
                }
                else if (lchild->getBalance() == 0)
                {
                    temp->setBalance(0);
                    temp->getRight()->setBalance(0);
                    lchild->setBalance(0);
                }
                else if (lchild->getBalance() == 1)
                {
                    temp->setBalance(-1);
                    temp->getRight()->setBalance(0);
                    lchild->setBalance(0);
                }
                removefix(parent, ndiff);
            }
        }
        else if ((temp->getBalance() + diff) == 1)
        {
            temp->setBalance(1);
            return;
        }
        else if ((temp->getBalance() + diff) == 0)
        {
            temp->setBalance(0);
            removefix(parent, ndiff);
        }
    }
}


template<class Key, class Value>
void AVLTree<Key, Value>::nodeSwap( AVLNode<Key,Value>* n1, AVLNode<Key,Value>* n2)
{
    BinarySearchTree<Key, Value>::nodeSwap(n1, n2);
    char tempB = n1->getBalance();
    n1->setBalance(n2->getBalance());
    n2->setBalance(tempB);
}


#endif
