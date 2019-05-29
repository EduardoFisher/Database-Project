//
// Created by lalof on 5/21/2019.
//

#ifndef MYDB_SQL_PARSER_H
#define MYDB_SQL_PARSER_H

#include "stokenizer.h"
#include <vector>
#include "map.h"
#include "multimap.h"
#include "queue.h"


class sql_parser
{
public:
    sql_parser(std::string operation);

//    bool more()
//    {
//        return (Tokens.empty() != 0);
//    }

//    bool done()
//    {
//        return (Tokens.size() == 0);
//    }

    int Col_Number(std::string curr_String)
    {
        if(this->map.contains(curr_String))
        {
            return this->map[curr_String];
        }
        return SYMBOL;
    }

    void set_up_map();

    bool set_MultiMap(MMap<std::string, std::string> &multimap);

    bool vaild(MMap<std::string, std::string> &MMap_create);

    void print_MMap(MMap<std::string, std::string> &multimap);

    void set_up_Table(int _Table[][MAX_COLUMNS]);

private:
    Queue<std::string> Tokens;
    MMap<std::string, std::string> mmap;
    Map<std::string, int> map;
    static int _Table[MAX_ROWS][MAX_COLUMNS];
};


#endif //MYDB_SQL_PARSER_H
