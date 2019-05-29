//
// Created by lalof on 5/21/2019.
//

#ifndef MYDB_MULTIMAP_H
#define MYDB_MULTIMAP_H

#include "multipair.h"
#include "bplustree.h"

template <typename K, typename V>
class MMap
{
public:
    typedef BPlusTree<MPair<K, V> > map_base;
    class Iterator
    {
    public:
        friend class MMap;
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

        MPair<K, V> operator *()
        {
            return MPair<K, V>((*_it).key, (*_it).value_list);
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

    MMap():mmap(true)
    {
        //mmap = BPlusTree<MPair<K, V> >(true);
    }

//  Iterators
    Iterator begin();

    Iterator end();

//  Capacity
    int size() const;
    bool empty() const;

//  Element Access
    const std::vector<V>& operator[](const K& key) const;
    std::vector<V>& operator[](const K& key);

//  Modifiers
    void insert(const K& k, const V& v);
    void erase(const K& key);
    void clear();

//  Operations:
    bool contains(const K& key) const ;
    bool contains(const K& key) ;
    std::vector<V> &get(const K& key)
    {
        return this->mmap.get(MPair<K, V>(key));
    }

    Iterator find(const K& key)
    {
        return this->mmap.find(MPair<K, V>(key));
    }

//    int count(const K& key);
    // I have not writtent hese yet, but we will need them:
    //    lower_bound
    Iterator lower_bound(const K& key)
    {
        Iterator it;
        for(it = this->mmap.begin(); it != this->mmap.end(); it++)
        {
            if(key <= (*it).key)
                return it;
        }
        return it;
//        return mmap.find(MPair<K, V>(key));
    }
    //    upper_bound
    //Returns an Iterator to the one bigger than key
    Iterator upper_bound(const K& key)
    {
        Iterator it;
        for(it = this->mmap.begin(); it != this->mmap.end(); it++)
        {
            if(key < (*it).key)
                return it;
        }
        return it;
//        return ++mmap.find(MPair<K, V>(key));
    }
    //    equal_range:

    int count(const K& key)
    {
        Iterator it = mmap.find(MPair<K, V>(key));
        return (*it).value_list.size();
    }

    bool is_valid();

    friend std::ostream& operator<<(std::ostream& os, const MMap<K, V>& print_me){
        os<<print_me.mmap<<std::endl;
        return os;
    }

private:
    BPlusTree<MPair<K, V> > mmap;
};

//  Capacity
template <typename K, typename V>
int MMap<K, V>::size() const
{
    return this->mmap.size();
}

template <typename K, typename V>
bool MMap<K, V>::empty() const
{
    return this->mmap.empty();
}

//  Iterators
template <typename K, typename V>
typename MMap<K, V>::Iterator MMap<K, V>::begin()
{
    return this->mmap.begin();
}

template <typename K, typename V>
typename MMap<K, V>::Iterator MMap<K, V>::end()
{
    return this->mmap.end();
}

//  Modifiers
template <typename K, typename V>
void MMap<K, V>::insert(const K &k, const V &v)
{
    this->mmap.insert(MPair<K, V>(k, v));
}

template <typename K, typename V>
void MMap<K, V>::erase(const K &key)
{
    this->mmap.remove(MPair<K, V>(key));
}

template <typename K, typename V>
void MMap<K, V>::clear()
{
    this->mmap.clear_tree();
}

//  Operations:
template <typename K, typename V>
bool MMap<K, V>::contains(const K &target) const
{
    return this->mmap.contains(MPair<K, V>(target));
}

template <typename K, typename V>
bool MMap<K, V>::contains(const K &target)
{
    return this->mmap.contains(MPair<K, V>(target));
}

template <typename K, typename V>
std::vector<V>& MMap<K, V>::operator [](const K& key)
{
    return this->mmap.get(MPair<K, V>(key)).value_list;
}

template <typename K, typename V>
const std::vector<V>& MMap<K, V>::operator [](const K& key) const
{
    if(mmap.contains(MPair<K, V>(key)))
        return this->mmap.get(MPair<K, V>(key)).value_list;
}

#endif //MYDB_MULTIMAP_H
