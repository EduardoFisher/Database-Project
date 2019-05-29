//
// Created by lalof on 5/21/2019.
//

#ifndef MYDB_ARRAY_UTILITY_FUNCTIONS_H
#define MYDB_ARRAY_UTILITY_FUNCTIONS_H

#include <vector>
#include <iostream>
#include <cassert>
#include <cmath>

template<typename ITEM_TYPE>
ITEM_TYPE maximal(const ITEM_TYPE& a, const ITEM_TYPE& b);              //return the larger of the two items //done
template<typename ITEM_TYPE>
void swap(ITEM_TYPE& a, ITEM_TYPE& b);                                  //swap the two items //done
template<typename ITEM_TYPE>
int index_of_maximal(ITEM_TYPE data[ ], int n);                         //return index of the largest item in data //done
template<typename ITEM_TYPE>
void ordered_insert(ITEM_TYPE data[ ], int& n, ITEM_TYPE entry);        //insert entry into the sorted array
//data with length n
template<typename ITEM_TYPE>
int first_ge(const ITEM_TYPE data[ ], int n, const ITEM_TYPE& entry);   //return the first element in data that is //done
//not less than entry
template<typename ITEM_TYPE>
void attach_item(ITEM_TYPE data[ ], int& n, const ITEM_TYPE& entry);    //append entry to the right of data
template<typename ITEM_TYPE>
void insert_item(ITEM_TYPE data[ ], int i, int& n, ITEM_TYPE entry);    //insert entry at index i in data
template<typename ITEM_TYPE>
void detach_item(ITEM_TYPE data[ ], int& n, ITEM_TYPE& entry);          //remove the last element in data and place
//it in entry
template<typename ITEM_TYPE>
void delete_item(ITEM_TYPE data[ ], int i, int& n, ITEM_TYPE& entry);   //delete item at index i and place it in entry
template<typename ITEM_TYPE>
void merge(ITEM_TYPE data1[ ], int& n1, ITEM_TYPE data2[ ], int& n2);   //append data2 to the right of data1
template<typename ITEM_TYPE>
void split(ITEM_TYPE data1[ ], int& n1, ITEM_TYPE data2[ ], int& n2);   //move n/2 elements from the right of data1
//and move to data2
template<typename ITEM_TYPE>
void copy_array(ITEM_TYPE dest[], const ITEM_TYPE src[], int size);     //copy src[] into dest[]
template<typename ITEM_TYPE>
void print_array(const ITEM_TYPE data[], int n);                        //print array data

template<typename ITEM_TYPE>
void shiftRight(ITEM_TYPE data[ ], int &n, int here);
template<typename ITEM_TYPE>
void shiftLeft(ITEM_TYPE data[ ], int &n, int here);
template<typename ITEM_TYPE>
bool check(ITEM_TYPE array[ ], int size);

//-------------- Vector Extra operators: --------------------------------------------------------------------------------//
template<typename ITEM_TYPE>
std::vector<ITEM_TYPE>& operator +=(std::vector<ITEM_TYPE>& list, const ITEM_TYPE& addme);  //ilst.push_back(addme)
template<typename ITEM_TYPE>
std::ostream& operator <<(std::ostream& os, const std::vector<ITEM_TYPE>& list);            //print vector list
//template<typename ITEM_TYPE>
//std::vector<ITEM_TYPE>& operator =(std::vector<ITEM_TYPE>& list, const ITEM_TYPE& addme);


//---------------------------------------------------FUNCTION IMPLEMENATIONS---------------------------------------------//
template<typename ITEM_TYPE>
ITEM_TYPE maximal(const ITEM_TYPE& a, const ITEM_TYPE& b)
{
    return((a > b) ? a : b);
}

template<typename ITEM_TYPE>
void swap(ITEM_TYPE& a, ITEM_TYPE& b)
{
    ITEM_TYPE temp;
    temp = a;
    a = b;
    b = temp;
}

template<typename ITEM_TYPE>
int index_of_maximal(ITEM_TYPE data[ ], int n)
{
    int index = 0;
    for(int i = 0; i < n; i++)
    {
        (data[index] < data[i] ? index = i : index = index);
    }
    return index;
}

template<typename ITEM_TYPE>
void ordered_insert(ITEM_TYPE data[ ], int& n, ITEM_TYPE entry)
{
    int i = first_ge(data, n, entry);
    insert_item(data, i, n, entry);
}

template<typename ITEM_TYPE>
int first_ge(const ITEM_TYPE data[ ], int n, const ITEM_TYPE& entry)
{
    int i;
    for(i = 0; i < n; i++)
    {
        if((data[i] >= entry))
        {
            return i;
        }
    }
    return i;
}

//append entry to the right of data
template<typename ITEM_TYPE>
void attach_item(ITEM_TYPE data[ ], int& n, const ITEM_TYPE& entry)
{
    data[n] = entry;
    n++;
}

//insert entry at index i in data
template<typename ITEM_TYPE>
void insert_item(ITEM_TYPE data[ ], int i, int& n, ITEM_TYPE entry)
{
//    if(n != 0)
//    {
//        for(int index = n+1; index >= i; index--)
//            data[index] = data[index-1];
//    }
//    data[i] = entry;
//    n++;
    ITEM_TYPE* thing = new ITEM_TYPE[n+1];
    for(int g = 0; g < n; g++)
        thing[g] = data[g];
    data[i] = entry;
    n++;
    for(int j = i+1; j < n; j++)
        data[j] = thing[j-1];
    delete[] thing;

}

//remove the last element in data and place
//it in entry
template<typename ITEM_TYPE>
void detach_item(ITEM_TYPE data[ ], int& n, ITEM_TYPE& entry)
{
    entry = data[n-1];
    n--;
}

template<typename ITEM_TYPE>
void delete_item(ITEM_TYPE data[ ], int i, int& n, ITEM_TYPE& entry)
{
    entry = data[i];
    for (int pos = i+1; pos < n; pos++)
        data[pos-1] = data[pos];
    n--;
//    return;
//    ITEM_TYPE* thing = new ITEM_TYPE[n];
//    for(int index = 0; index < n; index++)
//        thing[index] = data[index];
//    for(int index = i; index < n; index++)
//    {
//        data[index] = thing[index+1];
//    }
//    n--;
}

template<typename ITEM_TYPE>
void merge(ITEM_TYPE data1[ ], int& n1, ITEM_TYPE data2[ ], int& n2)
{
//    n1 += n2;
    for(int i = 0; i < n2; i++)
        data1[n1++] = data2[i];
//    for(int i = 0; i < n2; i++)
//        ((n1 % 2 == 0) ? (data1[i+n2] = data2[i]) : (data1[i+n2+1] = data2[i]));
    n2 = 0;
}

//move n/2 elements from the right of data1
//and move to data2
template<typename ITEM_TYPE>
void split(ITEM_TYPE data1[ ], int& n1, ITEM_TYPE data2[ ], int& n2)
{
    //odd or evenness has nothing to do with this.
    //div n by 2: integer div.
    //copy the SECOND half to the data2 array.
    //update both n1 and n2

    n2 = n1 / 2;
    for(int i = 0; i < n2; i++)
        ((n1 % 2 == 0) ? data2[i] = data1[i+n2] : data2[i] = data1[i+n2+1]);
    n1 -= n2;
}

template<typename ITEM_TYPE>
void copy_array(ITEM_TYPE dest[], const ITEM_TYPE src[], int size)
{
    for(int i = 0; i < size; i++)
        dest[i] = src[i];
}

template<typename ITEM_TYPE>
void print_array(const ITEM_TYPE data[], int n)
{

    std::cout << "--\n";
    for(int i = 0; i < n; i++)
    {
        std::cout << data[i] << std::endl;
    }
    std::cout << "--";
}

template<typename ITEM_TYPE>
void shiftRight(ITEM_TYPE data[ ], int &n, int here)
{
    for(int i = n; i > here; --i)
        data[i] = data[i-1];
    n--;
}

template<typename ITEM_TYPE>
void shiftLeft(ITEM_TYPE data[ ], int &n, int here)
{
    for(int j = here; j < n; j++)
        data[j] = data[j+1];
    n--;
}

template<typename ITEM_TYPE>
bool check(ITEM_TYPE array[ ], int size)
{
    if(size == 0)
        return true;
    for(int i = 0; i < size-1; i++)
        if(array[i] > array[i+1])
        {
            std::cout << array[i] << " | " << array[i+1] << std::endl;
            return false;
        }
    return true;
}

//-------------- Vector Extra operators: --------------------------------------------------------------------------------//
template<typename ITEM_TYPE>
std::vector<ITEM_TYPE>& operator +=(std::vector<ITEM_TYPE>& list, const ITEM_TYPE& addme)
{
    list.push_back(addme);
    return list;
}

template<typename ITEM_TYPE>
std::vector<ITEM_TYPE>& operator +=(std::vector<ITEM_TYPE>& list, const std::vector<ITEM_TYPE>& addme)
{
    for(unsigned int i = 0; i < addme.size(); i++)
        list.push_back(addme.at(i));
    return list;
}

template<typename ITEM_TYPE1, typename ITEM_TYPE2>
std::vector<ITEM_TYPE1>& operator +=(std::vector<ITEM_TYPE1>& list, const ITEM_TYPE2& addme)
{
    list.push_back(addme);
    return list;
}

template<typename ITEM_TYPE>
std::ostream& operator <<(std::ostream& os, const std::vector<ITEM_TYPE>& list)
{
    unsigned int i;
    for(i = 0; i < list.size() - 1; i++)
        os << list.at(i) << " ";
    os << list.at(i);
    return os;
}

#endif //MYDB_ARRAY_UTILITY_FUNCTIONS_H
