//
// Created by lalof on 5/21/2019.
//

#ifndef MYDB_LINK_LIST_FUNCTIONS_H
#define MYDB_LINK_LIST_FUNCTIONS_H

#include <cassert>
#include "node.h"

////////////////////////////////////////////////
///All the function prototypes I will be using.
////////////////////////////////////////////////

//initializes head to NULL
template <typename T>
node<T>* init_head(node<T>* &head);

//deletes all the nodes in the list
template<typename T>
void delete_all(node<T>*&head);

//true if head is NULL, false otherwise.
template <typename T>
bool empty_list(const node<T>* head);

//makes a copy of the list, returns a pointer to the last node:
template <typename T>
node<T>* copy_list(const node<T>* head, node<T>* & cpy);

//insert at the beginning of the list:
template <typename T>
node<T>* insert_head(node<T>* &head, T item);

//insert_after: if after is NULL, inserts at head
template <typename T>
node<T>* insert_after(node<T>* &head, node<T>*& after, const T& item);

//delete the node at the head of the list, reuturn the item:
template <typename T>
T delete_head(node<T>* &head);

//print the list and return outs
template<typename T>
std::ostream& print_list(const node<T>* head, std::ostream& os);

///////////////////////////////////
///Function definitions start here.
///////////////////////////////////

//initializes head to NULL
template <typename T>
node<T>* init_head(node<T>* &head)
{
    head = NULL;
    return head;
}

//deletes all the nodes in the list
template<typename T>
void delete_all(node<T>*&head)
{
    //First check to see if the empty.
    if(empty_list(head) == true)
    {
        //If the list is empty then the program will end.
//        assert(head != NULL);
    }
        //If the list is not empty then we can continue on.
    else
    {
        //Continually delete nodes until we only have head pointing to NULL.
        while(head != NULL)
        {
            delete_head(head);
        }
    }
}

//true if head is NULL, false otherwise.
template <typename T>
bool empty_list(const node<T>* head)
{
    if(head == NULL)
    {
        return true;
    }
    else
    {
        return false;
    }
}

//makes a copy of the list, returns a pointer to the last node:
template <typename T>
node<T>* copy_list(const node<T>* head, node<T>* &cpy)
{
    //Case 1: if you are trying to copy an empty list.
    if(empty_list(head) == true)
    {
        init_head(cpy);
        return cpy;
    }
        //Case 2: if you are trying to copy an non-empty list.
    else
    {
        //This can probably be consolidated into something better.
        const node<T>* curr = head;
        init_head(cpy);
        node<T>* curr_cpy = cpy;
        curr_cpy = insert_head(cpy, curr->_item);
        curr = curr->_next;
        while(curr != NULL)
        {
            curr_cpy = insert_after(cpy, curr_cpy, curr->_item);
            curr = curr->_next;
        }
        return curr_cpy;
    }
}

//insert at the beginning of the list:
template <typename T>
node<T>* insert_head(node<T>* &head, T item)
{
    //First create a new node.
    node<T>* temp = new node<T>(item);
    //Then set temps->next to head.
    temp->_next = head;
    //Finally set head to where temp is pointing to.
    head = temp;
    return head;
}

//insert_after: if after is NULL, inserts at head
template <typename T>
node<T>* insert_after(node<T>* &head, node<T>*& after, const T& item)
{
    node<T>* temp;
    if(head == NULL)
    {
        temp = insert_head(head, item);
    }
        //If after is equal to NULL.
    else if(after == NULL)
    {
        temp = insert_head(after, item);
    }
        //If after is not equal to NULL.
    else
    {
        temp = new node<T>(item);
        temp->_next = after->_next;
        after->_next = temp;
    }
    return temp;
}

//delete the node at the head of the list, reuturn the item:
template <typename T>
T delete_head(node<T>* &head)
{
    node<T>* hold;
    T temp_item;
    //First check to see if the list is empty.
    if(empty_list(head) == true)
    {
        //If the list is empty then the program will end.
        std::cout << "your dumbass";
        assert(head != NULL);
    }
        //If the list is not empty then we can continue on.
    else
    {
        //First hold the pointer for head and heads->item.
        hold = head;
        temp_item = hold->_item;
        //Then point head to heads->next.
        head = head->_next;
        //Finally delete the hold variable.
        delete hold;
    }
    //Now return the item.
    return temp_item;
}

//print the list and return os
template<typename T>
std::ostream& print_list(const node<T>* head, std::ostream& os)
{
    const node<T>* curr = head;
    os << "Start->";
    while(curr != NULL)
    {
        os << "[" << curr->_item << "]->";
        curr = curr->_next;
    }
    os << "End of List";
    return os;
}

#endif //MYDB_LINK_LIST_FUNCTIONS_H
