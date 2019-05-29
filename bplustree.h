//
// Created by lalof on 5/21/2019.
//

#ifndef MYDB_BPLUSTREE_H
#define MYDB_BPLUSTREE_H

#include <iostream>
#include <iomanip>
#include <vector>
#include "array_utility_functions.h"
template<typename T>
class BPlusTree
{
public:
    class Iterator{
    public:
        friend class BPlusTree;
        Iterator(BPlusTree<T>* _it=NULL, int _key_ptr = 0):it(_it), key_ptr(_key_ptr){}

        T operator *(){
            assert(it != NULL);
            assert(key_ptr < it->data_count);
            return it->data[key_ptr];
        }

        Iterator operator++(int un_used){
            Iterator hold = *this;
            if(key_ptr == it->data_count - 1)
            {
                it = it->next;
                key_ptr = 0;
            }
            else
                ++key_ptr;
            return hold;
        }

        Iterator operator++(){
            if(key_ptr == it->data_count - 1)
            {
                it = it->next;
                key_ptr = 0;
            }
            else
                ++key_ptr;
            return *this;
        }
        friend bool operator ==(const Iterator& lhs, const Iterator& rhs){return ((lhs.it == rhs.it) && (lhs.key_ptr == rhs.key_ptr)); }

        friend bool operator !=(const Iterator& lhs, const Iterator& rhs){return ((lhs.it != rhs.it) || (lhs.key_ptr != rhs.key_ptr));}
        void print_Iterator(){
            std::cout << "[" << it->data[key_ptr] << "]";
        }
        bool is_null(){return !it;}
    private:
        BPlusTree<T>* it;
        int key_ptr;
    };

    BPlusTree(bool dups = false);
    //big three:
    BPlusTree(const BPlusTree<T>& other);
    ~BPlusTree();
    BPlusTree<T>& operator =(const BPlusTree<T>& RHS);

    void insert(const T& entry);                //insert entry into the tree
    void remove(const T& entry);                //remove entry from the tree

    void clear_tree();                          //clear this object (delete all nodes etc.)
    void copy_tree(const BPlusTree<T>& other);  //copy other into this object
    void copy_tree(const BPlusTree<T>& other, std::vector<BPlusTree<T>* > &address);

    bool contains(const T& entry);              //true if entry can be found in the array

    T& get(const T& entry);                     //return a reference to entry in the tree
    const T& get(const T& entry) const;         //return a reference to entry in the tree
    T& get_existing(const T& entry);            //return a reference to entry in the tree
    Iterator find(const T& entry);              //return an iterator to this key. NULL if not there.

    int size() const ;                          //count the number of elements in the tree
    bool empty() const;                         //true if the tree is empty

    void print_tree(int level = 0,
                    std::ostream &os=std::cout) const; //print a readable version of the tree
    friend std::ostream& operator<<(std::ostream& os,
                                    const BPlusTree<T>& print_me){
        print_me.print_tree(0, os);
        return os;
    }
    bool is_valid();

    Iterator begin();
    Iterator end();

private:
    static const int MINIMUM = 1;
    static const int MAXIMUM = 2 * MINIMUM;

    bool dups_ok;                       //true if duplicate keys may be inserted
    int data_count;                     //number of data elements
    T data[MAXIMUM + 1];                //holds the keys
    int child_count;                    //number of children
    BPlusTree* subset[MAXIMUM + 2];     //subtrees
    BPlusTree* next;
    bool is_leaf() const
    {return child_count==0;}        //true if this is a leaf node

    T* find_ptr(const T& entry);        //return a pointer to this key. NULL if not there.

    size_t _size;                       //holds the size of all the elements in the tree.

    //insert element functions
    void loose_insert(const T& entry);  //allows MAXIMUM+1 data elements in the root
    void fix_excess(int i);             //fix excess of data elements in child i

    //remove element functions:
    void loose_remove(const T& entry);  //allows MINIMUM-1 data elements in the root

    void fix_shortage(int i);  //fix shortage of data elements in child i
    // and return the smallest key in this subtree
    BPlusTree<T>* get_smallest_node();
    void get_smallest(T& entry);        //entry := leftmost leaf
    void get_biggest(T& entry);         //entry := rightmost leaf
    void remove_biggest(T& entry);      //remove the biggest child of this tree->entry
    void transfer_left(int i);          //transfer one element LEFT from child i
    void transfer_right(int i);         //transfer one element RIGHT from child i
    void merge_with_next_subset(int i);    //merge subset i with subset i+1

};

template<typename T>
BPlusTree<T>::BPlusTree(const BPlusTree<T> &other)
{
    this->copy_tree(other);
}

template<typename T>
BPlusTree<T>::~BPlusTree()
{
    this->clear_tree();
}

template<typename T>
BPlusTree<T>& BPlusTree<T>::operator =(const BPlusTree<T>& RHS)
{
    if(this != &RHS)
    {
        this->clear_tree();
    }
    this->copy_tree(RHS);
    return *this;
}

template<typename T>
T& BPlusTree<T>::get(const T &entry)
{
//    T* found = this->find_ptr(entry);
//    if(found)
//        return *found;
//    else
//    {
//        this->insert(entry);
//        return &find_ptr(entry);
//    }
//    const bool debug = false;
    if (!contains(entry))
        insert(entry);
    return get_existing(entry);
}

//More constant than the other one.
template<typename T>
const T& BPlusTree<T>::get(const T &entry) const
{
    T* found = this->find_ptr(entry);
    if(found)
        return *found;
    else
        return NULL;
}

template<typename T>
T& BPlusTree<T>::get_existing(const T &entry)
{
    /*
     * ---------------------------------------------------------------
     * This routing explains plainly how the BPlusTree is organized.
     * i = first_ge
     *       |      found         |    !found         |
     * ------|--------------------|-------------------|
     * leaf  |  you're done       | it's not there    |
     *       |                    |                   |
     * ------|--------------------|-------------------|
     * !leaf | subset[i+1]->get() |  subset[i]->get() |
     *       | (inner node)       |                   |
     * ------|--------------------|-------------------|
     *       |                    |                   |
     *
     * ---------------------------------------------------------------
     *
     */
    //assert that entry is not in the tree.
    //assert(contains(entry));

    const bool debug = false;
    int i = first_ge(data, data_count, entry);
    bool found = (i < data_count && data[i] == entry);
    if (is_leaf())
    {
        if (found)
        {
            return data[i];
        }
        else
        {
            if (debug) std::cout<<"get_existing was called with nonexistent entry"<<std::endl;
            assert(found);
        }
    }
    if (found) //inner node
        return subset[i+1]->get(entry);
        //or just return true?
    else //not found yet...
        return subset[i]->get(entry);
}

template<typename T>
bool BPlusTree<T>::contains(const T &entry)
{
    int i = first_ge(this->data, this->data_count, entry);
    bool found = (i < data_count && data[i] == entry);

    if(found && this->is_leaf())
        return true;
    else if(found && !this->is_leaf())
        this->subset[i+1]->contains(entry);
    else if(!found && !this->is_leaf())
        this->subset[i]->contains(entry);
    else if(!found && this->is_leaf())
        return false;
}

template<typename T>
void BPlusTree<T>::copy_tree(const BPlusTree<T> &other, std::vector<BPlusTree<T> *> &address)
{
    //make sure to set the links up

//    assert(!other.empty());
    copy_array(this->data, other.data, other.data_count);
    this->data_count = other.data_count;
    this->child_count = other.child_count;
    for(int j = this->child_count - 1; j >= 0; j--)
    {
        this->subset[j] = new BPlusTree<T>(other.dups_ok);
        this->subset[j]->copy_tree(*other.subset[j], address);
        if(this->subset[j]->is_leaf() && (address.empty() && (j <= this->child_count - 1)))
        {
            this->subset[j]->next = this->subset[j+1];
            if(j == 0)
            {
                address.push_back(this->subset[j]);
            }
        }
        else if(this->subset[j]->is_leaf() && (j == this->child_count - 1))
        {
            if(address.size() != 0)
            {
                this->subset[j]->next = address.front();
                address.pop_back();
            }
        }
    }
}

template<typename T>
void BPlusTree<T>::copy_tree(const BPlusTree<T> &other)
{
    std::vector<BPlusTree<T>* > address;
    this->copy_tree(other, address);
}

template<typename T>
typename BPlusTree<T>::Iterator BPlusTree<T>::begin()
{
    if(this->empty())
        return BPlusTree<T>::Iterator(NULL);
    BPlusTree<T>::Iterator start;
    if(!this->is_leaf())
        start = this->subset[0]->begin();
    else
        start = BPlusTree<T>::Iterator(this, 0);
    return start;
}

template<typename T>
typename BPlusTree<T>::Iterator BPlusTree<T>::end()
{
    return BPlusTree<T>::Iterator(NULL);
}

//This needs to be changed for effeicenty(not how you spell it) sake.
//This should be handled within Insert and Remove.
//This should just call and return the size_t size variable within the class.
template<typename T>
int BPlusTree<T>::size() const
{
    int total = 0;
    if(!this->is_leaf())
        total = this->subset[0]->size();
    else
    {
        const BPlusTree<T>* temp = this;
        while(temp->next)
        {
            total += temp->data_count;
            temp = temp->next;
        }
        total += temp->data_count;
    }
    return total;
}

template<typename T>
BPlusTree<T>::BPlusTree(bool dups)
{
    dups_ok = dups;
//    std::cout << dups_ok << std::endl;
    data_count = 0;
    child_count = 0;
    //This step is porbably not needed.
    for(int i = 0; i < MAXIMUM + 2; i++)
        this->subset[i] = NULL;
    next = NULL;
}

template<typename T>
void BPlusTree<T>::clear_tree()
{
    if(!this->is_leaf())
        for(int i = 0; i < child_count; i++)
        {
            this->subset[i]->clear_tree();
            this->subset[i]->data_count = 0;
            this->subset[i]->child_count = 0;
            delete subset[i];
        }
    this->data_count = 0;
    this->child_count = 0;
    this->next = NULL;
}

template<typename T>
bool BPlusTree<T>::empty() const
{
    return (this->data_count == 0);
}

template<typename T>
void BPlusTree<T>::print_tree(int level, std::ostream &os) const
{
    //1. print the last child (if any)
    //2. print all the rest of the data and children
    if (child_count > 1)
        subset[child_count - 1]->print_tree(level + 1, os);
    for(int i = data_count-1; i >= 0; i--)
    {
        os << std::setw(level*5) << data[i] << std::endl;
        if(!is_leaf())
//            subset[(data_count - 1)- i]->print_tree(level + 1, os);
            subset[i]->print_tree(level + 1, os);
    }
}

template<typename T>
void BPlusTree<T>::insert(const T &entry)
{
    loose_insert(entry);
    if(this->data_count > MAXIMUM && this->is_leaf())
    {
        BPlusTree<T>* node = new BPlusTree<T>(dups_ok);
        copy_array(node->data, this->data, this->data_count);
        node->data_count = this->data_count;
        this->data_count = 0;
        this->child_count = 1;
        this->subset[0] = node;
        fix_excess(0);

    }
    else
    {
        if(this->data_count > MAXIMUM)
        {
            //Create a new BTree.
            BPlusTree<T>* newTree = new BPlusTree<T>(dups_ok);
            //copy everything over the newTree including all the subsets!
            copy_array(newTree->data, this->data, this->data_count);
            copy_array(newTree->subset, this->subset, this->child_count);
            //Make sure to set the newTree's child and data counter to the roots one.
            newTree->data_count = this->data_count;
            newTree->child_count = this->child_count;
            //now set this roots child counter and data count to 0.
            this->data_count = 0;
            this->child_count = 1;
            //The point the root subset[0] to newTree
            //And add +1 to the child counter because we now have one.
            this->subset[0] = newTree;
            //Now we can call fix_excess on subset[0];
            fix_excess(0);
        }
    }
}

template<typename T>
void BPlusTree<T>::loose_insert(const T &entry)
{
    int i = first_ge(this->data, this->data_count, entry);
    bool found = (i < data_count && data[i] == entry);

    if(this->is_leaf() && found)
    {
        if(dups_ok == true)
        {
            //make sure to return true;
            this->_size++;
            data[i] = data[i] + entry;
            //return true;
        }
        else
        {
            //And make sure to return false.
            //This should be the only case that returns false.
            data[i] = entry;
            //return false;
        }
    }
    else if(this->is_leaf() && !found)
    {
        //This case would return true;
        ordered_insert(this->data, this->data_count, entry);
        this->_size++;
        //return true;
    }
    else if(!this->is_leaf() && found)
    {
        subset[i+1]->loose_insert(entry);
    }
    else
    {
        this->subset[i]->loose_insert(entry);
        if(this->subset[i]->data_count > MAXIMUM)
            this->fix_excess(i);
    }
}

template<typename T>
void BPlusTree<T>::fix_excess(int i)
{
    if(subset[i]->is_leaf())
    {
        T item;
        insert_item(this->subset, i+1, this->child_count, new BPlusTree<T>(dups_ok));
        split(this->subset[i]->data, this->subset[i]->data_count, this->subset[i+1]->data, this->subset[i+1]->data_count);
        detach_item(this->subset[i]->data, this->subset[i]->data_count, item);
//        delete_item(this->subset[i+1]->data, 0, this->subset[i+1]->data_count, item);
//        attach_item(this->subset[i]->data, this->subset[i]->data_count, item);
        insert_item(this->subset[i+1]->data, 0,this->subset[i+1]->data_count, item);
        ordered_insert(data, this->data_count, item);
        this->subset[i+1]->next = this->subset[i]->next;
        this->subset[i]->next = this->subset[i+1];
    }
    else
    {
        T item;
        //Add a new Tree to subset[i+1] used in the spilting process.
        insert_item(this->subset, i+1, this->child_count, new BPlusTree<T>(dups_ok));
        //Spilt both the subset array and the data array between subset[i] and subset[i+1]
        split(this->subset[i]->data, this->subset[i]->data_count,
              this->subset[i+1]->data, this->subset[i+1]->data_count);
        split(this->subset[i]->subset, this->subset[i]->child_count,
              this->subset[i+1]->subset, this->subset[i+1]->child_count);
        //Remove the last item from subset[i] and insert it into data.
        detach_item(this->subset[i]->data, this->subset[i]->data_count, item);
        ordered_insert(this->data, this->data_count, item);
    }
}

template<typename T>
void BPlusTree<T>::remove(const T &entry)
{
    loose_remove(entry);
    if(this->data_count < MINIMUM && !this->is_leaf())
    {
        //Fix the root of the entire tree so that it no longer has zero entries.
        //Our solution to this problem is to first set a temporay pointer that points
        //to the only child. Next, copy everything from the only child up to the root node.
        //Finally, delete the original child.
        BPlusTree<T>* tree = subset[0];
        copy_array(this->data, tree->data, tree->data_count);
        copy_array(this->subset, tree->subset, tree->child_count);
        this->data_count = tree->data_count;
        this->child_count = tree->child_count;
        tree->child_count = 0;
        delete tree;
    }
}

template<typename T>
void BPlusTree<T>::loose_remove(const T &entry)
{
    //First check to see if we even have this item in the Tree.
    T hold;
    int i = first_ge(data, data_count, entry);
    bool found = (i<data_count && data[i] == entry);
    if(found && this->is_leaf())
        delete_item(this->data, i, this->data_count, hold);
    else if(found && !this->is_leaf())
    {
        subset[i+1]->loose_remove(entry);
        if(subset[i+1]->data_count < MINIMUM)
            this->fix_shortage(i+1);

        i = first_ge(data, data_count, entry);
        found = (i<data_count && data[i] == entry);
        if(found)
            this->subset[i+1]->get_smallest(this->data[i]);
        else
        {
            int j = first_ge(this->subset[i]->data, this->subset[i]->data_count, entry);
            bool found2 = (j<this->subset[i]->data_count && this->subset[i]->data[j] == entry);
            if(found2)
                this->subset[i]->subset[j+1]->get_smallest(this->subset[i]->data[j]);
        }
    }
    else if(!found && !this->is_leaf())
    {
        subset[i]->loose_remove(entry);
        if(subset[i]->data_count < MINIMUM)
            this->fix_shortage(i);
    }
    else if(!found && this->is_leaf())
        return;
}

template<typename T>
void BPlusTree<T>::fix_shortage(int i)
{
    if(this->subset[i]->is_leaf())
    {
        //Special B+Tree way.
        if(i > 0 && this->subset[i-1]->data_count > MINIMUM)
        {
            this->transfer_right(i);
        }
        else if(i < this->child_count - 1 && this->subset[i+1]->data_count > MINIMUM)
        {
            this->transfer_left(i);
        }
        else if(i > 0 && this->subset[i-1] && this->subset[i-1]->data_count == MINIMUM)
        {
            merge_with_next_subset(i-1);
        }
        else if(i < this->child_count - 1 && this->subset[i+1]->data_count == MINIMUM)
        {
            merge_with_next_subset(i);
        }
    }
    else if(!this->subset[i]->is_leaf())
    {
        if(i > 0 && this->subset[i-1]->data_count > MINIMUM)
        {
            T item;
            //a). Transfer data[i-1] down to the front of subset[i]->data
            //Remember to shift over existing entries to make room, and add one to subset[i]->data_count
            delete_item(this->data, i-1, this->data_count, item);
            ordered_insert(this->subset[i]->data, this->subset[i]->data_count, item);
            //b). Transfer the final item of subset[i-1]->data up to replace data[i-1],
            //and subtract one from subset[i-1]->child_count.
            detach_item(this->subset[i-1]->data, this->subset[i-1]->data_count, item);
            ordered_insert(this->data, this->data_count, item);
            //c). If subset[i-1] has children, transfer the final child of subset[i-1] over
            //to the front of subset[i]. This will involve shifting over the array.
            if(!this->subset[i-1]->is_leaf())
            {
                BPlusTree<T>* node;
                delete_item(this->subset[i-1]->subset, MAXIMUM, this->subset[i-1]->child_count, node);
                //            attach_item(this->subset[i]->subset, this->subset[i]->child_count, node);
                insert_item(this->subset[i]->subset, 0, this->subset[i]->child_count, node);
            }
        }
            //Case 2: Transfer an extra entry from subset[i+1].
            //This works the same as Case 1: shown above.
        else if(i < this->child_count - 1 && this->subset[i+1]->data_count > MINIMUM)
        {
            T item;

            delete_item(this->data, i, this->data_count, item);
            ordered_insert(this->subset[i]->data, this->subset[i]->data_count, item);
            delete_item(this->subset[i+1]->data, 0, this->subset[i+1]->data_count, item);
            ordered_insert(this->data, this->data_count, item);
            //a). Transfer data[i] down to the front of subset[i]->data
            //Remember to shift over existing entries to make room, and add one to subset[i]->data_count

            //b). Transfer the final item of subset[i+1]->data up to replace data[i+1],
            //and subtract one from subset[i+1]->child_count.

            //c). If subset[i+1] has children, transfer the final child of subset[i+1] over
            //to the front of subset[i]. This will involve shifting over the array.
            if(!this->subset[i+1]->is_leaf())
            {
                BPlusTree<T>* node;
                delete_item(this->subset[i+1]->subset, 0, this->subset[i+1]->child_count, node);
                attach_item(this->subset[i]->subset, this->subset[i]->child_count, node);
            }
        }
            //Case 3: Combine subset[i] with subset[i-1]
            //Suppose that subset[i-1] is present(in other words i > 0), but it has only MINIMUM entries
            //In this case, we cannot tranfer an entry from subset[i-1], but we can combine subset[i] with subset[i-1]
            //The combining occurs in three steps:
        else if(i > 0 && this->subset[i-1]->data_count == MINIMUM)
        {
            merge_with_next_subset(i-1);
        }
            //Case 4: Combine subset[i] with subset[i+1]
            //This case is basically the same as Case 3 shown above.
        else if(i < this->child_count - 1 && (subset[i + 1]->data_count == MINIMUM))
        {
            merge_with_next_subset(i);
        }
    }
}

template<typename T>
void BPlusTree<T>::transfer_left(int i) //transfer one element LEFT from child i
{
    T item;
    delete_item(this->subset[i+1]->data, 0, subset[i+1]->data_count, item);
    attach_item(this->subset[i]->data, this->subset[i]->data_count, item);
    delete_item(this->data, i, this->data_count, item);
    ordered_insert(this->data, this->data_count, this->subset[i+1]->data[0]);

}

template<typename T>
void BPlusTree<T>::transfer_right(int i) //transfer one element RIGHT from child i
{
    T item;
    delete_item(this->subset[i-1]->data, this->subset[i-1]->data_count-1, this->subset[i-1]->data_count, item);
    ordered_insert(this->subset[i]->data, this->subset[i]->data_count, item);
    delete_item(this->data, i, this->data_count, item);
    ordered_insert(this->data, this->data_count, this->subset[i]->data[0]);
}

template<typename T>
void BPlusTree<T>::merge_with_next_subset(int i) //merge subset i with subset i+1
{
    T item;
    BPlusTree<T>* node;
    if(this->subset[i]->is_leaf())
    {
        delete_item(this->data, i, this->data_count, item);
        merge(this->subset[i]->data, this->subset[i]->data_count, this->subset[i+1]->data, this->subset[i+1]->data_count);
        delete_item(this->subset, i+1, this->child_count, node);
        this->subset[i]->next = node->next;
        delete node;
    }
    else
    {
        delete_item(this->data, i, this->data_count, item);
        attach_item(this->subset[i]->data, this->subset[i]->data_count, item);
        merge(this->subset[i]->data, this->subset[i]->data_count, this->subset[i+1]->data, this->subset[i+1]->data_count);
        merge(this->subset[i]->subset, this->subset[i]->child_count, this->subset[i+1]->subset, this->subset[i+1]->child_count);
        delete_item(this->subset, i+1, this->child_count, node);
        delete node;
    }
}

template<typename T>
void BPlusTree<T>::get_smallest(T &entry)
{
    if(this->is_leaf())
        entry = data[0];
    else
        this->subset[0]->get_smallest(entry);
}

template<typename T>
void BPlusTree<T>::get_biggest(T &entry)
{
    if(this->is_leaf())
        entry = data[this->data_count - 1];
    else
        this->subset[this->child_count - 1]->get_biggest(entry);
}

template<typename T>
bool BPlusTree<T>::is_valid()
{
    if(this->empty())
        return true;
    if(!this->is_leaf())
    {
        int i = 0;
        subset[i++]->is_valid();
        while(child_count > i)
            subset[i++]->is_valid();
        if(this->data_count > 1)
        {
            int check = 0;
            int follower = 0;
            while(follower < this->data_count - 1)
            {
                if(data[follower] < data[++follower])
                    check++;
            }
            if(check + 1 != this->data_count)
                return false;
        }
        return true;
    }
    else
    {
        if(data_count > 1)
        {
            int check2 = 0;
            int follower2 = 0;
            while(follower2 < this->data_count - 1)
            {
                if(data[follower2] < data[++follower2])
                    check2++;
            }
            if(check2 + 1 != this->data_count)
                return false;
        }
    }
}

template<typename T>
T* BPlusTree<T>::find_ptr(const T &entry)
{
    int i = first_ge(this->data, this->data_count, entry);
    bool found = (i < data_count && data[i] == entry);

    if(found && this->is_leaf())
        return &this->data[i];
    else if(found && !this->is_leaf())
        this->subset[i+1]->find_ptr(entry);
    else if(!found && !this->is_leaf())
        this->subset[i]->find_ptr(entry);
    else if(!found && this->is_leaf())
        return NULL;
}

template<typename T>
typename BPlusTree<T>::Iterator BPlusTree<T>::find(const T& entry)
{
    int i = first_ge(this->data, this->data_count, entry);
    bool found = (i < data_count && data[i] == entry);

    if(found && this->is_leaf())
        return BPlusTree<T>::Iterator(this, i);
    else if(found && !this->is_leaf())
        this->subset[i+1]->find(entry);
    else if(!found && !this->is_leaf())
        this->subset[i]->find(entry);
    else if(!found && this->is_leaf())
        return BPlusTree<T>::Iterator(NULL);
}

#endif //MYDB_BPLUSTREE_H
