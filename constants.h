//
// Created by lalof on 5/21/2019.
//

#ifndef MYDB_CONSTANTS_H
#define MYDB_CONSTANTS_H

#include <string>

const int MAX_COLUMNS = 256;
const int MAX_ROWS = 100;

const int MAX_BUFFER = 1000;

const int DIGITS_START_STATE = 0;
const char DIGITS[] = "0123456789";

const int SPACE_START_STATE = 6;

const int ALPHA_START_STATE = 4;
const char ALPHA[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";

const std::string SELECT = "Select";
const int SELECT_START_STATE_P = 1;
const int SELECT_START_STATE = 10;

const std::string ASTERISK = "*";
const int ASTERISK_START_STATE_P = 2;
const int ASTERISK_START_STATE = 11;

const int COMMA_START_STATE = 12;

const int START = 0;
const int FROM = 3;
const int COMMA = 12;
const int SYMBOL = 4;
const int SYMBOL_I = 5;
const int FEILDNAME = 44;
const int TABLENAME = 45;
const int FIND = 46;
const int FEILD_COMP_I = 47;

const int WHERE = 20;
const int COMP_FEILD = 21;
const int OPERATOR = 22;
const int LEFT_QUOTE = 23;
const int RIGHT_QUOTE = 24;
const int ANOTHER_STRING = 25;
const int AND_PLUS_OR = 26;
const int FEILDNAME_I = 80;
const int FEILDNAME_C = 85;

const int LEFT_QUOTE_I = 27;
const int INSERT = 50;
const int INTO = 51;
const int TABLENAME_I = 52;

const int PRINT = 70;

const int LOAD  = 72;
const int DROP = 73;
const int CREATE = 74;
const int END = 99;

//const std::string PRINT = "Print";
const std::string INTO_P = "into";
const std::string SELECT_P = "Select";
const std::string INSERT_P = "Insert";
const std::string WHERE_P = "where";
const std::string OPERATOR_P = "OPERATOR";
const std::string ASTERISK_P = "ASTERISK";
const std::string FEILD_LIST = "FIELDLIST";
const std::string FEILD_COMP = "FIELDNAME";
const std::string COMMAND = "COMMAND";
const std::string TABLE_NAME = "TABLENAME";
const std::string FIND_P = "Find";
const std::string LOAD_P = "Load";
const std::string DROP_P = "Drop";
const std::string CREATE_P = "Create";
const std::string PRINT_P = "Print";
const std::string AND_OR = "and/or";

const std::string YES = "Yes";
const std::string NO = "No";
const std::string END_P = "END";


#endif //MYDB_CONSTANTS_H
