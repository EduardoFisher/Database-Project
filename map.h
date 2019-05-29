//
// Created by lalof on 5/21/2019.
//

#ifndef MYDB_MAP_H
#define MYDB_MAP_H

#include "bplustree.h"
#include "pair.h"

template<typename K, typename V>
class Map
{
public:
    typedef BPlusTree<Pair<K, V> > map_base;
    class Iterator
    {
    public:
        friend class Map;
        Iterator(typename map_base::Iterator it = typename map_base::Iterator(NULL))
        {
            _it = it;
        }

        Iterator operator ++(int unused)
        {
            return _it++;
        }

        Iterator operator ++()
        {
            Iterator hold = _it;
            ++_it;
            return hold;
        }

        Pair<K, V> operator *()
        {
            return Pair<K, V>((*_it).key, (*_it).value);
        }

        friend bool operator ==(const Iterator& lhs, const Iterator& rhs)
        {
            return (lhs._it == rhs._it);
        }

        friend bool operator !=(const Iterator& lhs, const Iterator& rhs)
        {
            return (lhs._it != rhs._it);
        }

    private:
        typename map_base::Iterator _it;

    };

    Map()
    {
        map = BPlusTree<Pair<K, V> >(false);
    }
//  Iterators
    Iterator begin();
    Iterator end();

//  Capacity
    int size() const;
    bool empty() const;

//  Element Access
    V& operator[](const K& key);
    V& at(const K& key);
    const V& at(const K& key) const;


//  Modifiers
    void insert(const K& k, const V& v);
    void erase(const K& key);
    void clear();
    V get(const K& key);

//  Operations:
    Iterator find(const K& key);
    bool contains(const Pair<K, V> &target) const;
    bool contains(const Pair<K, V> &target);

    // I have not written these yet, but we will need them:
    //    int count(const K& key);
    //    lower_bound
    Iterator lower_bound(const K& key)
    {
        return map.find(Pair<K, V>(key));
    }
    //    upper_bound
    Iterator upper_bound(const K& key)
    {
        return ++map.find(Pair<K, V>(key));
    }
    //    equal_range:

    bool is_valid(){return map.is_valid();}

    friend std::ostream& operator<<(std::ostream& os, const Map<K, V>& print_me){
        os << print_me.map << std::endl;
        return os;
    }
private:
    int key_count;
    BPlusTree<Pair<K, V> > map;
};

//  Iterators
template <typename K, typename V>
typename Map<K, V>::Iterator Map<K, V>::begin()
{
    return this->map.begin();
}

template <typename K, typename V>
typename Map<K, V>::Iterator Map<K, V>::end()
{
    return this->map.end();
}

//  Capacity
template <typename K, typename V>
int Map<K, V>::size() const
{
    return this->map.size();
}

template <typename K, typename V>
bool Map<K, V>::empty() const
{
    return this->map.empty();
}

//  Element Access
template <typename K, typename V>
V& Map<K, V>::operator [](const K& key)
{
    return this->map.get(Pair<K, V>(key)).value;
}

template <typename K, typename V>
const V& Map<K, V>::at(const K &key) const
{
    if(this->map.contains(Pair<K, V>(key)))
        return this->map.get(Pair<K, V>(key));
    return NULL;
}

template <typename K, typename V>
V& Map<K, V>::at(const K &key)
{
    return this->map(Pair<K, V>(key));
}

//  Modifiers
template <typename K, typename V>
void Map<K, V>::insert(const K &k, const V &v)
{
    this->map.insert(Pair<K, V>(k, v));
    this->key_count++;
}

template <typename K, typename V>
void Map<K, V>::erase(const K &key)
{
    this->map.remove(Pair<K, V>(key));
    this->key_count--;
}

template <typename K, typename V>
void Map<K, V>::clear()
{
    this->map.clear_tree();
}

template <typename K, typename V>
V Map<K, V>::get(const K& key)
{
    return this->map.get_existing(Pair<V, K>(key));
}

//  Operations:
template <typename K, typename V>
typename Map<K, V>::Iterator Map<K, V>::find(const K& key)
{
    return this->map.find(Pair<K, V>(key));
}

template <typename K, typename V>
bool Map<K, V>::contains(const Pair<K, V> &target) const
{
    return this->map.contains(target);
}

template <typename K, typename V>
bool Map<K, V>::contains(const Pair<K, V> &target)
{
    return this->map.contains(target);
}

#endif //MYDB_MAP_H
