//
// Created by lalof on 5/21/2019.
//

#ifndef MYDB_MULTIPAIR_H
#define MYDB_MULTIPAIR_H

#include "array_utility_functions.h"

template <typename K, typename V>
struct MPair
{
    K key;
    std::vector<V> value_list;

    //--------------------------------------------------------------------------------
    /*      MPair CTORs:
     *  must have these CTORs:
     *  - default CTOR / CTOR with a key and no value: this will create an empty vector
     *  - CTOR with a key AND a value: pushes the value into the value_list
     *  _ CTOR with a key and a vector of values (to replace value_list)
     */
    //--------------------------------------------------------------------------------
    MPair(const K& k=K())
    {
        key = k;
    }

    MPair(const K& k, const V& v)
    {
        key = k;
        value_list += v;
    }

    MPair(const K& k, const std::vector<V>& vlist)
    {
        key = k;
        value_list = vlist;
    }

    //--------------------------------------------------------------------------------

    //You'll need to overlod << for your vector:
    friend std::ostream& operator <<(std::ostream& os, const MPair<K, V>& print_me)
    {
        os << "[" << print_me.key << ":" << print_me.value_list << "]";
        return os;
    }

    friend bool operator ==(const MPair<K, V>& lhs, const MPair<K, V>& rhs)
    {
        return (lhs.key == rhs.key);
    }

    friend bool operator < (const MPair<K, V>& lhs, const MPair<K, V>& rhs)
    {
        return (lhs.key == rhs.key);
    }

    friend bool operator <= (const MPair<K, V>& lhs, const MPair<K, V>& rhs)
    {
        return (lhs.key <= rhs.key);
    }

    friend bool operator > (const MPair<K, V>& lhs, const MPair<K, V>& rhs)
    {
        return (lhs.key > rhs.key);
    }

    friend bool operator >= (const MPair<K, V>& lhs, const MPair<K, V>& rhs)
    {
        return (lhs.key >= rhs.key);
    }

    friend MPair<K, V> operator + (const MPair<K, V>& lhs, const MPair<K, V>& rhs)
    {
        MPair<K, V> temp(lhs.key, lhs.value_list);

        temp.value_list.push_back(rhs.value_list.at(0));
        return temp;
    }

//    MPair<K, V>& operator + ( const MPair<K, V>& rhs)
//    {
//        key = rhs.key;
//        for(int i = 0; i <rhs.value_list.size() ; i++)
//        value_list.push_back(rhs.value_list[i]);
//        return *this;
//    }
};

#endif //MYDB_MULTIPAIR_H
