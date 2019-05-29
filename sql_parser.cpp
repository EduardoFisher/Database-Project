//
// Created by lalof on 5/21/2019.
//

#include "sql_parser.h"
#include "statemachine.h"
#include <iomanip>

using namespace std;

int sql_parser::_Table[MAX_ROWS][MAX_COLUMNS];

void sql_parser::set_up_map()
{
    map["Select"] = SELECT_START_STATE_P;
    map["Insert"] = INSERT;
    map["Print"] = PRINT;
    map["Load"] = LOAD;
    map["Drop"] = DROP;
    map["Create"] = CREATE;
    map["into"] = INTO;
    map["*"] = ASTERISK_START_STATE_P;
    map[","] = COMMA;
    map["from"] = FROM;
    map["End"] = END;

    map["<="] = OPERATOR;
    map[">="] = OPERATOR;
    map["<"] = OPERATOR;
    map[">"] = OPERATOR;
    map["="] = OPERATOR;
    map["where"] = WHERE;
    map[" "] = SPACE_START_STATE;
    map["\""] = LEFT_QUOTE;
    map["and"] = AND_PLUS_OR;
    map["or"] = AND_PLUS_OR;
}

void sql_parser::print_MMap(MMap<string, string> &multimap)
{
    MMap<string, string>::Iterator it;
//    it = multimap.begin();
    for(it = multimap.begin();  it != multimap.end(); it++)
    {
        cout << (*it) << endl;
    }
}

sql_parser::sql_parser(std::string operation)
{
    this->set_up_Table(_Table);
    STokenizer stk(operation);
    Token t;

    stk >> t;

    while(stk.more())
    {
        Tokens.push(t.token_str());
        t = Token();
        stk >> t;
    }
    Tokens.push(t.token_str());
    this->set_up_map();
}

bool sql_parser::set_MultiMap(MMap<string, string>& multimap)
{
    multimap.clear();
    return this->vaild(multimap);
}

bool sql_parser::vaild(MMap<string, string>& MMap_create)
{
    bool Debug = false;
    int state = START;
    bool insert = false;
    bool create = false;
    while(/*!this->done()*/!this->Tokens.empty() && state != -1)
    {
        string curr_string = this->Tokens.pop();
//                this->Tokens.front();
        if(curr_string == INSERT_P)
            insert = true;
        if(curr_string == CREATE_P)
            create = true;
//        Tokens.pop();
        int col = Col_Number(curr_string);
        if(col == LEFT_QUOTE && insert == true)
            col = LEFT_QUOTE_I;
        if(state == FEILDNAME_C && create == false)
            state = FEILDNAME;
        if(col!=SPACE_START_STATE)
        {
            state = _Table[state][col];
            if(Debug)
            {
                cout << "=============================\n";
                cout << "Current string: " << curr_string << endl;
                cout << "C:" << col << endl;
                cout << "R:" << state << endl;
                cout << "S:" << state << endl;
                cout << "==============================\n";
            }
            if(state == SELECT_START_STATE_P)
                MMap_create[COMMAND]+=curr_string;
            else if(state == INSERT)
                MMap_create[COMMAND]+=curr_string;
            else if(state == PRINT)
                MMap_create[COMMAND]+=curr_string;
            else if(state == LOAD)
                MMap_create[COMMAND]+=curr_string;
            else if(state == DROP)
                MMap_create[COMMAND]+=curr_string;
            else if(state == CREATE)
                MMap_create[COMMAND]+=curr_string;
            else if(state == ASTERISK_START_STATE_P)
                MMap_create[ASTERISK_P]+=curr_string;
            else if(state == FEILDNAME)
            {
                MMap_create[FEILD_LIST]+=curr_string;
            }
            else if(state == FEILDNAME_C)
            {
                MMap_create[FEILD_LIST]+=curr_string;
            }
            else if(state == TABLENAME)
                MMap_create[TABLE_NAME]+=curr_string;
            else if(state == OPERATOR)
                MMap_create[OPERATOR_P]+=curr_string;
            else if (state == FEILD_COMP_I)
                MMap_create[FEILD_COMP]+=curr_string;
            else if (state == ANOTHER_STRING)
                MMap_create[FIND_P]+=curr_string;
            else if(state == WHERE)
                MMap_create[WHERE_P]+=YES;
            else if(state == END)
                MMap_create[END_P]+=YES;
            else if(state == AND_PLUS_OR)
            {
                if(!MMap_create.contains(AND_OR))
                    MMap_create[AND_OR]+=YES;
                MMap_create[AND_OR]+=curr_string;
            }
            else
            {
                //do nothing, bc they are not worthy.
            }
        }
    }
    if(!MMap_create.contains("where"))
        MMap_create[WHERE_P]+=NO;
    if(!MMap_create.contains("End"))
        MMap_create[END_P]+=NO;
    if(!MMap_create.contains(AND_OR))
        MMap_create[AND_OR]+=NO;
    return is_success(_Table, state);
}



void sql_parser::set_up_Table(int _Table[][MAX_COLUMNS])
{
    init_table(_Table);
    //set upfor the table:
    mark_fail(_Table, START);
    //set up where everything goes in this table:
    //Command "SELECT"
    mark_fail(_Table, SELECT_START_STATE_P);
    mark_cell(START, _Table, SELECT_START_STATE_P, SELECT_START_STATE_P);

    mark_cell(SELECT_START_STATE_P, _Table, ASTERISK_START_STATE_P, ASTERISK_START_STATE_P);
    mark_cell(SELECT_START_STATE_P, _Table, SYMBOL, FEILDNAME);
    mark_cell(FEILDNAME, _Table, COMMA, COMMA);
    mark_cell(COMMA, _Table, SYMBOL, FEILDNAME);
    mark_cell(ASTERISK_START_STATE_P, _Table, FROM, FROM);
    mark_cell(FEILDNAME, _Table, FROM, FROM);
    mark_cell(FROM, _Table, SYMBOL, TABLENAME);
    mark_cell(TABLENAME, _Table, WHERE, WHERE);
    mark_cell(WHERE, _Table, SYMBOL, FEILD_COMP_I);
    mark_cell(FEILD_COMP_I, _Table, OPERATOR, OPERATOR);
    mark_cell(OPERATOR, _Table, LEFT_QUOTE, LEFT_QUOTE);
    mark_cell(LEFT_QUOTE, _Table, SYMBOL, ANOTHER_STRING);
    mark_cell(ANOTHER_STRING, _Table, LEFT_QUOTE, RIGHT_QUOTE);
    mark_cell(RIGHT_QUOTE, _Table, AND_PLUS_OR, AND_PLUS_OR);
    mark_cell(AND_PLUS_OR, _Table, SYMBOL, FEILD_COMP_I);

    //Insert into Table_name, "value1", ->, "valueN"
    mark_cell(START, _Table, INSERT, INSERT);
    mark_cell(INSERT, _Table, INTO, INTO);
    mark_cell(INTO, _Table, SYMBOL, TABLENAME);
    mark_cell(TABLENAME, _Table, LEFT_QUOTE_I, LEFT_QUOTE_I);
    mark_cell(LEFT_QUOTE_I, _Table, SYMBOL, FEILDNAME);
    mark_cell(FEILDNAME, _Table, LEFT_QUOTE_I, RIGHT_QUOTE);
    mark_cell(RIGHT_QUOTE, _Table, COMMA, COMMA);
    mark_cell(COMMA, _Table, LEFT_QUOTE_I, LEFT_QUOTE_I);

    //Print Command
    mark_cell(START, _Table, PRINT, PRINT);
    mark_cell(PRINT, _Table, SYMBOL, TABLENAME);

    //Load Command
    mark_cell(START, _Table, LOAD, LOAD);
    mark_cell(LOAD, _Table, SYMBOL, TABLENAME);

    //Drop Command
    mark_cell(START, _Table, DROP, DROP);
    mark_cell(DROP, _Table, SYMBOL, TABLENAME);

    //Create Command
    mark_cell(START, _Table, CREATE, CREATE);
    mark_cell(CREATE, _Table, SYMBOL, TABLENAME);
    mark_cell(TABLENAME, _Table, SYMBOL, FEILDNAME_C);
    mark_cell(FEILDNAME_C, _Table, COMMA, COMMA);
    mark_cell(COMMA, _Table, SYMBOL, FEILDNAME_C);


    //End Command
    mark_cell(START, _Table, END, END);
    mark_success(_Table, END);

    mark_success(_Table, TABLENAME);
    mark_success(_Table, ASTERISK_START_STATE_P);
    mark_success(_Table, RIGHT_QUOTE);
    mark_success(_Table, FEILDNAME_C);

    mark_fail(_Table, WHERE);
    mark_fail(_Table, COMP_FEILD);
    mark_fail(_Table, OPERATOR);
    mark_fail(_Table, LEFT_QUOTE);
    mark_fail(_Table, ANOTHER_STRING);
    mark_fail(_Table, FROM);
    mark_fail(_Table, START);
    mark_fail(_Table, FEILDNAME);
    mark_fail(_Table, COMMA);
    mark_fail(_Table, SELECT_START_STATE_P);
    mark_fail(_Table, AND_PLUS_OR);

    mark_fail(_Table, INTO);

}
