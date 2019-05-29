//
// Created by lalof on 5/21/2019.
//

#ifndef MYDB_DATABASE_H
#define MYDB_DATABASE_H

#include "sql_parser.h"
#include "table.h"

class DataBase
{
public:
    friend class Table;
    DataBase();
    void run();

    bool Does_File_Exist(std::string name);
    bool Table_Loaded(std::string name, int &pos);
    void Create_File(std::string name);
private:
    void Loaded_Tables();
//    Map<std::string, Table> _Tables;
    unsigned int size;
    std::vector<Table*> _Tables;
};


#endif //MYDB_DATABASE_H
