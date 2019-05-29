//
// Created by lalof on 5/21/2019.
//

#ifndef MYDB_STACK_H
#define MYDB_STACK_H

#include "link_list_functions.h"

template<typename T>
class Stack
{
public:
    Stack():_top(NULL){}

    //Big 3;
    ~Stack();
    Stack(const Stack<T>& other);
    Stack<T>& operator = (const Stack<T>& other);

    void push(T item);
    T pop();
    T top();
    bool empty();

    friend std::ostream& operator << (std::ostream& os, const Stack& s)
    {
        return print_list(s._top, os);
    }

private:
    node<T>* _top;
};

///////////////////////////////////////////////////
///Start of the function definintions
///////////////////////////////////////////////////

///////////////////////////////////////////////////
///Start of the Big 3 Functions
///////////////////////////////////////////////////

template<typename T>
Stack<T>::~Stack()
{
    if(_top != NULL)
    {
        delete_all(_top);
        this->_top = NULL;
    }
}

template<typename T>
Stack<T>::Stack(const Stack<T> &other)
{
    copy_list(_top, other);
}

template<typename T>
Stack<T>& Stack<T>::operator =(const Stack& other)
{
    if(this != &other)
    {
        delete_all(_top);
        copy_list(other._top, _top);
    }

    return *this;
}

///////////////////////////////////////////////////
///End of the Big 3 Functions
///////////////////////////////////////////////////

template<typename T>
void Stack<T>::push(T item)
{
    insert_head(_top, item);
}

template<typename T>
T Stack<T>::pop()
{
    return delete_head(_top);
}

template<typename T>
bool Stack<T>::empty()
{
    return empty_list(_top);
}

template<typename T>
T Stack<T>::top()
{
    return _top->_item;
}

#endif //MYDB_STACK_H
