//
// Created by lalof on 5/21/2019.
//

#ifndef MYDB_TABLE_H
#define MYDB_TABLE_H

#include <vector>
#include "multimap.h"
#include "map.h"
#include "queue.h"
#include "stack.h"
#include <fstream>


//Table(Name, Fieldlist) creates a table with fieldlist;
//Table(Name)
class Table
{
public:
    //ONLY CALL THIS ONE WHEN WE KNOW THE FILE EXISTS.
    Table(std::string name);
    //ONLU CALL THIS ONE WHEN WE ARE CREATING A NEW FILE.
    Table(std::string name, std::vector<std::string> &mapping);
    //Commands:
    void insert(std::vector<std::string>& field_name);
    int Count();
    void show();
    void Create_Table(std::string name);
    Table* Select(std::vector<std::string>& field_names, std::vector<std::string> FieldName, std::vector<std::string> op, std::vector<std::string> value, std::vector<std::string> and_or);
    Table* Select(std::vector<std::string> &field_names, std::string FieldName, std::string op, std::string value);
    Table* Select_All(std::vector<std::string>& field_names);

    bool Check_Field_List(std::vector<std::string> v);
    bool Check_Field_Comp(std::vector<std::string> v);

    void Drop_Table(std::string name);
    void Load_Table(std::string name);

    std::string Get_Name();
    bool Does_Exist(std::string name);

    void print_FieldMap();

    void show_MultiMap_Values();
    Table Search_Single(std::string field_name);
    void Create_Index_Field_List(std::vector<std::string> field_names, std::vector<int> &index_from_field_list);
//    void Create_Input(vector<string> field_names, Record r, vector<string>& somethingFields, vector<int> index_from_field_list);

    Queue<std::string> Shunting_Yard(std::vector<std::string> FieldName, std::vector<std::string> op, std::vector<std::string> value,
                                     std::vector<std::string> and_or);
    std::vector<int> RPN(Queue<std::string> &input);
    //This is only called when the Table(name) CTOR is called.
    void Reindex();

    void DeleteTemp();

    void Save_Table();
private:
    std::string _name;
    int count;
    std::vector<std::string> Fields;
    MMap<std::string, int> search_Map[10];
    Map<std::string, int> fieldMap;
};

#endif //MYDB_TABLE_H
