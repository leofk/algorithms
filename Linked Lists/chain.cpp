#include "chain.h"
#include "chain_given.cpp"
#include <cmath>
#include <iostream>

// PA1 functions

/**
 * Destroys the current Chain. This function should ensure that
 * memory does not leak on destruction of a chain.
 */
Chain::~Chain()
{
    clear();
}

/**
 * Inserts a new node at the back of the List.
 * This function **SHOULD** create a new ListNode.
 *
 * @param ndata The data to be inserted.
 */
void Chain::insertBack(const Block &ndata)
{
    Node * listNode = new Node(ndata); 
    listNode->prev = tail_->prev;
    listNode->next = tail_; 
    tail_->prev->next = listNode;
    tail_->prev = listNode;
    length_ += 1; 
}


/**
 * Swaps the two nodes at the indexes "node1" and "node2".
 * The indexes of the nodes are 1 based.
 * assumes i and j are valid (in {1,length_} inclusive)
 * 
 */
void Chain::swap(int i, int j)
{ 
    if (i == j) {return; }
    
    Node * nodeI = walk(head_, i);
    Node * nodeJ = walk(head_, j); 

    Node * nodeINext = nodeI->next; 
    Node * nodeIPrev = nodeI->prev; 
    Node * nodeJNext = nodeJ->next; 
    Node * nodeJPrev = nodeJ->prev; 
    
    if (i+1 == j) { // when i < j & consecutive
        nodeI->next = nodeJNext; 
        nodeI->prev = nodeJ; 
        nodeJ->next = nodeI; 
        nodeJ->prev = nodeIPrev; 
    
        nodeJNext->prev = nodeI; 
        nodeIPrev->next = nodeJ; 
    } else if (j+1 == i) { // when j < i & consecutive
        nodeJ->next = nodeINext; 
        nodeJ->prev = nodeI; 
        nodeI->next = nodeJ; 
        nodeI->prev = nodeJPrev; 
    
        nodeINext->prev = nodeJ; 
        nodeJPrev->next = nodeI;  
    } else { // when not consecutive
        nodeI->next = nodeJNext;
        nodeI->prev = nodeJPrev;
        nodeJ->next = nodeINext;
        nodeJ->prev = nodeIPrev;
    
        nodeINext->prev = nodeJ;
        nodeIPrev->next = nodeJ;
        nodeJNext->prev = nodeI;
        nodeJPrev->next = nodeI;
    }
}

/**
 * Reverses the chain
 */
void Chain::reverse()
{
    Node * temp;
    Node * listNode = head_;
    
    while (listNode != NULL) {
        temp = listNode->next;
        listNode->next = listNode->prev;
        listNode->prev = temp;
        listNode = temp;
    }

    temp = tail_;
    tail_ = head_;
    head_ = temp;
}

/*
* Modifies the current chain by "rotating" every k nodes by one position.
* In every k node sub-chain, remove the first node, and place it in the last 
* position of the sub-chain. If the last sub-chain has length less than k,
* then don't change it at all. 
* Some examples with the chain a b c d e:
*   k = 1: a b c d e
*   k = 2: b a d c e
*   k = 3: b c a d e
*   k = 4: b c d a e
*/
void Chain::rotate(int k)
{
    int index = 1;

    if (k == 1) {return;}

    for (int i = k; i <= length_; i += k) {
        for (int j = 1; j < k; j++) {
            swap(index, index+1);
            index++;
        }
        index++;
    }
}

/**
 * Destroys all dynamically allocated memory associated with the
 * current Chain class.
 */
void Chain::clear()
{
    Node * curr = head_;
    Node * temp;

    while (curr != tail_) {
        temp = curr->next;
        curr->next = NULL;
        curr->prev = NULL;
        delete curr;
        curr = temp;
    }

    delete tail_;
    tail_ = NULL;
}

/* makes the current object into a copy of the parameter:
 * All member variables should have the same value as
 * those of other, but the memory should be completely
 * independent. This function is used in both the copy
 * constructor and the assignment operator for Chains.
 */
void Chain::copy(Chain const &other)
{
    length_ = other.length_; 
    numRows_ = other.numRows_; 
    numCols_ = other.numCols_; 
    height_ = other.height_; 
    width_ = other.width_;  

    head_ = new Node();
    tail_ = new Node();
    head_->next = tail_;
    tail_->prev = head_;
    head_->prev = NULL;
    tail_->next = NULL;

    Node * nextCopy = other.head_->next;
    Node * curr = head_;
    Node * temp;

    while (nextCopy != other.tail_){
        temp = new Node(nextCopy->data);
        curr->next = temp;
        temp->prev = curr;
        curr = temp;
        nextCopy = nextCopy->next;
    }

    curr->next = tail_;
    tail_->prev = curr;
}

/***********************************
 * swapColumns
 * parameters: 1 <= i <= numCols_
 *              1 <= j <= numCols_
 *
 * Swaps the positions of columns i and j
 * in the original grid of blocks by
 * moving nodes in the chain.
 *
 ***********************************/

void Chain::swapColumns(int i, int j) {
    for (int x = 0; x < numRows_; x++) {
        swap(i, j);
        i += numCols_;
        j += numCols_;
    }
}

/***********************************
 * swapRows
 * parameters: 1 <= i <= numRows_
 *              1 <= j <= numRows_
 *
 * Swaps the positions of rows i and j
 * in the original grid of blocks by
 * moving nodes in the chain.
 *
 ***********************************/
void Chain::swapRows(int i, int j) {
    i += (numCols_ - 1) * (i - 1);
    j += (numCols_ - 1) * (j - 1);

    for (int x = 0; x < numCols_; x++) {
        swap(i, j);
        i++;
        j++;
    }
}


