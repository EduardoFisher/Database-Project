//
// Created by lalof on 5/21/2019.
//

#include "table.h"
#include "binary_file.h"
#include <stdio.h>
#include <iomanip>

using namespace std;

const string ADD_ON = "_FieldList.txt";
const string TEMP_TABLE = "TempTable";

vector<int> intersection(vector<int>& v1, vector<int>& v2);
vector<int> Union(vector<int>& v1, vector<int>& v2);

Table::Table(string name)
{
    int i = 0;
    ifstream f;
    string tempname = name;
    string line;
    tempname += ADD_ON;
    _name = name;
    f.open(tempname);
    if(f.is_open())
    {
        while(getline(f,line))
        {
            fieldMap[line] = i;
            Fields.push_back(line);
            i++;
        }
        f.close();
    }
    this->Reindex();
}

Table::Table(string name, std::vector<string>& mapping)
{
    ofstream f;
    string tempname = name;
    _name = name;
    for(unsigned int i = 0; i < mapping.size(); i++)
    {
        Fields.push_back(mapping.at(i));
        fieldMap[mapping.at(i)] = i;
    }
    tempname += ADD_ON;
    f.open(tempname);
    for(unsigned int i = 0; i < mapping.size(); i++)
        f << mapping.at(i) << endl;
    //this->search_Map.resize(mapping.size());
    f.close();
}

void Table::insert(std::vector<std::string>& field_name)
{
    fstream f;
    Record r(field_name, this->fieldMap.size());
    open_fileW(f ,this->_name.c_str());
    int index;
    f.seekg(0, f.end);
    index = r.write(f);
    for(unsigned int i = 0; i < field_name.size(); i++)
        search_Map[i][field_name[i]] += index;
    f.close();
}

void Table::show_MultiMap_Values()
{
    MMap<string, int>::Iterator it ;
    for(int i = 0; i < this->fieldMap.size(); i++)
    {
        for( it = search_Map[i].begin(); it != search_Map[i].end(); it++)
        {
            cout << (*it) << endl;
        }
    }

}

Table* Table::Select_All(vector<string>& field_names)
{
    bool DEBUG = false;
    fstream f;
    vector<int> index_from_field_list;

    //Modify this function so that it handles errors or create a new function to do it for you.
    this->Create_Index_Field_List(field_names, index_from_field_list);
    Record records;
    if(field_names.at(0) == "*")
        records = Record(this->fieldMap.size());
    else
        records = Record(field_names.size());
    if(DEBUG)
    {
        cout << "Field Names\n";
        cout << field_names << endl;
        cout << "Field Names size: " << field_names.size() << endl;
    }
    Table* tempTable;
    if(field_names.at(0) == "*")
        tempTable = new Table("TempTable", this->Fields);
    else
        tempTable = new Table("TempTable", field_names);
    vector<string> somethingFields;
    open_fileRW(f, this->_name.c_str());

    int pos = 0;
    while(true)
    {
        records.read(f, pos);
        if(f.eof())break;
        if(field_names.at(0) == "*")
        {
            int size = this->fieldMap.size();
            for(int j = 0; j < size; j++)
                somethingFields.push_back(records.getRow(index_from_field_list.at(j)));
        }
        else
        {
            for(unsigned int j = 0; j < field_names.size(); j++)
                somethingFields.push_back(records.getRow(index_from_field_list.at(j)));
        }
        tempTable->insert(somethingFields);
        somethingFields.clear();
        pos++;
    }
    return tempTable;
}

//You must complete the reindexing function before this!
//build table with this
//Lname Fname age, Fname, =, name;
Table* Table::Select(vector<string>& field_names, string FieldName, string op, string value)
{
    const bool DEBUG = false;
    fstream f;
    vector<int> thingy;
    vector<int> index_from_field_list;
    int index = 0;
    this->Create_Index_Field_List(field_names, index_from_field_list);

    if(this->fieldMap.contains(FieldName))
        index = this->fieldMap[FieldName];

    if(DEBUG)
    {
        cout << "Index from field list: \n";
        cout << index_from_field_list << endl;
    }
    if(DEBUG)
        cout << "index: " << index << endl;



    if(op == "=")
        thingy = this->search_Map[index][value];
    else if(op == ">")
    {
        for(MMap<std::string, int>::Iterator it = this->search_Map[index].upper_bound(value); it != this->search_Map[index].end(); it++)
            thingy += (*it).value_list;
    }
    else if(op == ">=")
    {
        MMap<std::string, int>::Iterator it;
        for(it = this->search_Map[index].lower_bound(value); it != this->search_Map[index].end(); it++)
            thingy += (*it).value_list;
    }
    else if(op == "<")
    {
        for(MMap<std::string, int>::Iterator it = this->search_Map[index].begin(); it != this->search_Map[index].lower_bound(value); it++)
            thingy += (*it).value_list;
    }
    else if(op == "<=")
    {
        MMap<std::string, int>::Iterator it;
        for(it = this->search_Map[index].begin(); it != this->search_Map[index].upper_bound(value); it++)
            thingy += (*it).value_list;
//        thingy += (*it).value_list;
    }

    if(DEBUG)
    {
        cout << "Thingy\n";
        cout << thingy << endl;
    }

    Record records;
    if(field_names.at(0) == "*")
    {
        records.Size_setter(this->fieldMap.size());
    }
    else
    {
        records.Size_setter(field_names.size());
    }
    if(DEBUG)
    {
        cout << "Field Names\n";
        cout << field_names << endl;
        cout << "Field Names size: " << field_names.size() << endl;
    }

    Table* tempTable;
    if(field_names.at(0) == "*")
        tempTable = new Table("TempTable", Fields);
    else
        tempTable = new Table("TempTable", field_names);
    vector<string> somethingFields;
    open_fileRW(f, this->_name.c_str());

    if(DEBUG)
        cout << "Thingy size: " << thingy.size() << endl;
    for(unsigned int i = 0; i < thingy.size(); i++)
    {
        records.read(f, thingy.at(i));
        if(field_names.at(0) == "*")
        {
            int size = this->fieldMap.size();
            for(int j = 0; j < size; j++)
                somethingFields.push_back(records.getRow(index_from_field_list.at(j)));
        }
        else
        {
            for(unsigned int j = 0; j < field_names.size(); j++)
                somethingFields.push_back(records.getRow(index_from_field_list.at(j)));
        }
        tempTable->insert(somethingFields);
        somethingFields.clear();
    }
    return tempTable;
}
Table* Table::Select(vector<string>& field_names, vector<string> FieldName, vector<string> op, vector<string> value, std::vector<string> and_or)
{
    const bool DEBUG = false;
    fstream f;
    vector<int> thingy;
    vector<int> index_from_field_list;
    vector<int> index;
    Queue<string> the_shunted;
    this->Create_Index_Field_List(field_names, index_from_field_list);

    the_shunted = this->Shunting_Yard(FieldName, op, value, and_or);
    thingy = this->RPN(the_shunted);

    if(DEBUG)
    {
        cout << "Index from field list: \n";
        cout << index_from_field_list << endl;
    }
    if(DEBUG)
        cout << "index: " << index << endl;

    if(DEBUG)
    {
        cout << "Thingy\n";
        cout << thingy << endl;
    }

    Record records;
    if(field_names.at(0) == "*")
    {
        records.Size_setter(this->fieldMap.size());
    }
    else
    {
        records.Size_setter(field_names.size());
    }
    if(DEBUG)
    {
        cout << "Field Names\n";
        cout << field_names << endl;
        cout << "Field Names size: " << field_names.size() << endl;
    }

    Table* tempTable;
    if(field_names.at(0) == "*")
        tempTable = new Table("TempTable", Fields);
    else
        tempTable = new Table("TempTable", field_names);
    vector<string> somethingFields;
    open_fileRW(f, this->_name.c_str());

    if(DEBUG)
        cout << "Thingy size: " << thingy.size() << endl;
    for(unsigned int i = 0; i < thingy.size(); i++)
    {
        records.read(f, thingy.at(i));
        if(field_names.at(0) == "*")
        {
            int size = this->fieldMap.size();
            for(int j = 0; j < size; j++)
                somethingFields.push_back(records.getRow(index_from_field_list.at(j)));
        }
        else
        {
            for(unsigned int j = 0; j < field_names.size(); j++)
                somethingFields.push_back(records.getRow(index_from_field_list.at(j)));
        }
        tempTable->insert(somethingFields);
        somethingFields.clear();
    }
    return tempTable;
}


//sizeof file / bytes for one record
void Table::show()
{
    Record r(this->fieldMap.size());
    fstream f;
    open_fileRW(f, this->_name.c_str());
//    f.seekg(0);
    int i = 0;
    int count = 1;
//    std::cout << "========";
    while(true)
    {
        r.read(f, i);
        if(f.eof())break;
        cout << r;
//        std::cout << "========";
        if(count == 3)
        {
            cout << endl;
            count = 0;
        }
        count++;
        i++;
    }
    cout << endl;
    f.close();
}

//creat update insex

//Table&

string Table::Get_Name()
{
    return this->_name;
}

void Table::print_FieldMap()
{
    for(Map<string, int>::Iterator it = fieldMap.begin(); it != fieldMap.end(); it++)
        cout << (*it) << endl;
}

int Table::Count()
{
    streampos end;
    fstream f;
    Record r;
    open_fileRW(f, this->_name.c_str());
//    begin = f.tellg();
    f.seekg(0, f.end);
//    size = f.tellg();
    end = f.tellg();
    f.close();
    return ( sizeof(r.size())/ end);
}

bool Table::Does_Exist(string name)
{
    ifstream f(name.c_str());
    if(!f)
        return false;
    return true;
}

void Table::Reindex()
{
    const bool DEBUG = false;
    fstream f;
    Record r;
    int i = 0;
    int size = this->fieldMap.size();
    open_fileRW(f, this->_name.c_str());
    while(true)
    {
        r.read(f, i);
        if(f.eof()) break;
        for(int j = 0; j < size; j++)
            search_Map[j][r.getRow(j)] += i;
        i++;
    }
    if(DEBUG)
        this->show_MultiMap_Values();
}

void Table::Create_Index_Field_List(vector<string> field_names, vector<int> &index_from_field_list)
{
    if(field_names.at(0) == "*")
    {
        int size = this->fieldMap.size();
        for(int i = 0; i < size; i++)
        {
            index_from_field_list.push_back(i);
        }
    }
    else
    {
        for(unsigned int i = 0; i < field_names.size(); i++)
        {
            string tempString = field_names.at(i);
            if(this->fieldMap.contains(tempString))
                index_from_field_list.push_back(this->fieldMap[tempString]);
        }
    }
}

bool Table::Check_Field_Comp(std::vector<string> v)
{
    for(unsigned int i = 0; i < v.size(); i++)
    {
        bool is_there = false;
        for(unsigned int j = 0; j < this->Fields.size(); j++)
        {
            if(v.at(i) == this->Fields.at(j))
                is_there = true;
        }
        if(is_there == false)
            return false;
    }
    return true;
}

bool Table::Check_Field_List(std::vector<string> v)
{
    for(unsigned int i = 0; i < v.size(); i++)
    {
        bool is_there = false;
        for(unsigned int j = 0; j < this->Fields.size(); j++)
        {
            if(v.at(i) == this->Fields.at(j))
                is_there = true;
        }
        if(is_there == false)
            return false;
    }
    return true;
}

void Table::DeleteTemp()
{
    string temp = TEMP_TABLE;
    remove(temp.c_str());
    temp += ADD_ON;
    remove(temp.c_str());
}

Queue<string> Table::Shunting_Yard(std::vector<string> FieldName, std::vector<string> op, std::vector<string> value, std::vector<string> and_or)
{
    Queue<string> rpn;
    //It doesn't matter which one i choose bc Fieldname and value are exactly the same.
    for(unsigned int i = 0; i < FieldName.size(); i++)
    {
        if((and_or.size()) > 1 && i == 0)//for the case we have and/or case.
        {
            //push
            rpn.push(FieldName.at(i));
            rpn.push(value.at(i));
            rpn.push(op.at(i));
        }
        else
        {
            rpn.push(FieldName.at(i));
            rpn.push(value.at(i));
            rpn.push(op.at(i));
            rpn.push(and_or.at(i));
        }
    }
    return rpn;
}

vector<int> Table::RPN(Queue<string> &input)
{
    Stack<string> l;
    Stack<vector<int> > thingy;
    string what;
    Map<string, int> op;
    op["<="] = 0;
    op[">="] = 1;
    op["<"] = 2;
    op[">"] = 3;
    op["="] = 4;
    Map<string, int> logic;
    logic["and"] = 5;
    logic["or"] = 6;
    int index;
    while(!input.empty())
    {
        what = input.pop();
        if(op.contains(what))
        {
            if(what == "=")
            {
                string thingy1;
                string thingy2;
                thingy1 = l.pop();
                thingy2 = l.pop();
                index = this->fieldMap[thingy2];
                thingy.push(this->search_Map[index][thingy1]);
            }
            else if(what == "<")
            {
                vector<int> hold;
                string thingy1;
                string thingy2;
                thingy1 = l.pop();
                thingy2 = l.pop();
                index = this->fieldMap[thingy2];
                for(MMap<std::string, int>::Iterator it = this->search_Map[index].begin(); it != this->search_Map[index].lower_bound(thingy1); it++)
                    hold += (*it).value_list;
                thingy.push(hold);
            }
            else if(what == "<=")
            {
                vector<int> hold;
                string thingy1;
                string thingy2;
                thingy1 = l.pop();
                thingy2 = l.pop();
                index = this->fieldMap[thingy2];
                MMap<std::string, int>::Iterator it;
                for(it = this->search_Map[index].begin(); it != this->search_Map[index].upper_bound(thingy1); it++)
                    hold += (*it).value_list;
                thingy.push(hold);
            }
            else if(what == ">")
            {
                vector<int> hold;
                string thingy1;
                string thingy2;
                thingy1 = l.pop();
                thingy2 = l.pop();
                index = this->fieldMap[thingy2];
                for(MMap<std::string, int>::Iterator it = this->search_Map[index].upper_bound(thingy1); it != this->search_Map[index].end(); it++)
                    hold += (*it).value_list;
                thingy.push(hold);
            }
            else if(what == ">=")
            {
                vector<int> hold;
                string thingy1;
                string thingy2;
                thingy1 = l.pop();
                thingy2 = l.pop();
                index = this->fieldMap[thingy2];
                MMap<std::string, int>::Iterator it;
                for(it = this->search_Map[index].lower_bound(thingy1); it != this->search_Map[index].end(); it++)
                    hold += (*it).value_list;
                thingy.push(hold);
            }
        }
        else
        {
            if(logic.contains(what))
            {
                if(what == "and")
                {
                    //find intersection
                    vector<int> result;
                    vector<int> v1;
                    vector<int> v2;
                    v1 = thingy.pop();
                    v2 = thingy.pop();
                    result = intersection(v1, v2);
                    thingy.push(result);
                }
                else
                {
                    //find the union
                    vector<int> result;
                    vector<int> v1;
                    vector<int> v2;
                    v1 = thingy.pop();
                    v2 = thingy.pop();
                    result = Union(v1, v2);
                    thingy.push(result);
                }
            }
            else
                l.push(what);
        }
    }
    return thingy.pop();
}

vector<int> intersection(vector<int>& v1, vector<int>& v2)
{
    vector<int> l;
    for(unsigned int i = 0; i < v1.size(); i++)
    {
        for(unsigned int j = 0; j < v2.size(); j++)
        {
            if(v1.at(i) == v2.at(j))
                l.push_back(v1.at(i));
        }
    }
    return l;
}

vector<int> Union(vector<int>& v1, vector<int>& v2)
{
    vector<int> l;
    bool dup = false;
    l = v1;
//    for(unsigned int i = 0; i < v1.size(); i++)
//    {
//        l.push_back(v1.at(i));
//    }
    for(unsigned int i = 0; i < v2.size(); i++)
    {
        dup = false;
        for(unsigned int j = 0; j < l.size(); j++)
        {
            if(v2.at(i) == l.at(j))
            {
                dup = true;
                break;
            }
        }
        if(dup == false)
            l.push_back(v2.at(i));
    }
    return l;
}

//We could write our very own vector search function to make this process faster.
//Note within this function v1 is always the
vector<int> search(vector<int>& v1, vector<int>& v2)
{

}
