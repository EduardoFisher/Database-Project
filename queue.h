//
// Created by lalof on 5/21/2019.
//

#ifndef MYDB_QUEUE_H
#define MYDB_QUEUE_H

#include "link_list_functions.h"

template <typename T>
class Queue
{
public:
    Queue():_head(NULL), _tail(NULL){}

    //Big 3.
    ~Queue();
    Queue(Queue<T> &other);
    Queue<T> &operator =(const Queue &rhs);

    void push(T item);
    T pop();
    bool empty();
    T front();

    friend std::ostream& operator <<(std::ostream& os, const Queue& q)
    {
        return print_list(q._head, os);

    }

private:
    node<T>* _head;
    node<T>* _tail;
};

///////////////////////////////////////////////////
///Start of the function definintions
///////////////////////////////////////////////////

///////////////////////////////////////////////////
///Start of the Big 3 Functions
///////////////////////////////////////////////////

//Destructor.
template<typename T>
Queue<T>::~Queue()
{
    if(_head != NULL)
    {
        delete_all(_head);
        this->_tail = NULL;
        this->_head = NULL;
    }

}

//Copy Constructor.
template<typename T>
Queue<T>::Queue(Queue<T> &other)
{
    _tail = copy_list(_head, other._head);
}

//Assignment Operator.
template<typename T>
Queue<T>& Queue<T>::operator =(const Queue& rhs)
{
    if(this != &rhs)
    {
        delete_all(_head);
        _tail = copy_list(rhs._head, _head);
    }

    return *this;
}

///////////////////////////////////////////////////
///End of the Big 3 Functions
///////////////////////////////////////////////////

template<typename T>
void Queue<T>::push(T item)
{
    if(_head == NULL)
    {
        _tail = insert_head(_head, item);
    }
    else
    {
        _tail = insert_after(_head, _tail, item);
    }
}

template<typename T>
T Queue<T>::pop()
{
    return delete_head(_head);
}

template<typename T>
bool Queue<T>::empty()
{
    return empty_list(_head);
}

template<typename T>
T Queue<T>::front()
{
    return _head->_item;
}

#endif //MYDB_QUEUE_H
