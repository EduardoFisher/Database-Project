//
// Created by lalof on 5/21/2019.
//

#ifndef MYDB_PAIR_H
#define MYDB_PAIR_H

#include <iostream>

template <typename K, typename V>
struct Pair
{
    K key;
    V value;

    Pair(const K& k=K(), const V& v=V())
    {
        key = k;
        value = v;
    }

    friend std::ostream& operator <<(std::ostream& os, const Pair<K, V>& print_me)
    {
        os << "[" << print_me.key << ":" << print_me.value << "]";
        return os;
    }

    friend bool operator ==(const Pair<K, V>& lhs, const Pair<K, V>& rhs)
    {
        return (lhs.key == rhs.key);
    }

    friend bool operator < (const Pair<K, V>& lhs, const Pair<K, V>& rhs)
    {
        return (lhs.key < rhs.key);
    }

    friend bool operator > (const Pair<K, V>& lhs, const Pair<K, V>& rhs)
    {
        return (lhs.key > rhs.key);
    }

    friend bool operator <= (const Pair<K, V>& lhs, const Pair<K, V>& rhs)
    {
        return (lhs.key <= rhs.key);
    }
    friend bool operator >= (const Pair<K, V>& lhs, const Pair<K, V>& rhs)
    {
        return (lhs.key >= rhs.key);
    }

    friend Pair<K, V> operator + (const Pair<K, V>& lhs, const Pair<K, V>& rhs)
    {
        return Pair<K, V>(lhs.key, rhs.value);
    }
};

#endif //MYDB_PAIR_H
