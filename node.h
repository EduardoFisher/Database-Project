//
// Created by lalof on 5/21/2019.
//

#ifndef MYDB_NODE_H
#define MYDB_NODE_H

#include <iostream>

template <class T>
struct node
{
    T _item;
    node<T>* _next;
    node(const T& item = T(), node<T>* next = NULL):_item(item), _next(next){}
    friend std::ostream& operator << (std::ostream& os, const node<T>& print_me)
    {
        os << "[" << print_me._item << "]->";
        return os;
    }
};

#endif //MYDB_NODE_H
