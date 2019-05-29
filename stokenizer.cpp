//
// Created by lalof on 5/21/2019.
//

#include "stokenizer.h"
#include "statemachine.h"
#include <cstring>
#include <cassert>

using namespace std;

int STokenizer::_table[MAX_ROWS][MAX_COLUMNS];

STokenizer::STokenizer(char str[])
{
    assert(strlen(str) <= MAX_BUFFER);
    make_table(_table);
    set_string(str);
    _pos = 0;
}

STokenizer::STokenizer(string str)
{
    _pos = 0;
    make_table(_table);
    unsigned int i = 0;
    for(i = 0; i < str.size(); i++)
        _buffer[i] = str[i];
    _buffer[i] = '\0';
}

bool STokenizer::done()
{
    if(strlen(_buffer) < (unsigned int) _pos)
        return true;
    return false;
}

bool STokenizer::more()
{
    if(strlen(_buffer) <= (unsigned int) _pos)
        return false;
    return true;
}

STokenizer& operator >> (STokenizer& s, Token& t)
{
    string token = "";
    char c = s._buffer[s._pos];

    if(s._table[DIGITS_START_STATE][(int)c] == 1 ||
       s._table[DIGITS_START_STATE][(int)c] == 2)
    {
        s.get_token(DIGITS_START_STATE, token);
        if(token == ".")
        {
            s._pos++;
            t = Token(token, -1);
        }
        else
            t = Token(token, 2);
    }
    else if(s._table[ALPHA_START_STATE][(int)c] == 5)
    {
        s.get_token(ALPHA_START_STATE, token);
        t = Token(token, 1);
    }
    else if(s._table[SPACE_START_STATE][(int)c] == 6)
    {
        token += c;
        t = Token(token, 3);
        s._pos++;

    }
    else if(s._table[ASTERISK_START_STATE][(int)c] == 11)
    {
        token += c;
        t = Token(token, 4);
        s._pos++;

    }
    else if(s._table[COMMA_START_STATE][(int)c] == 12)
    {
        token += c;
        t = Token(token, 5);
        s._pos++;
    }
    else
    {
        token += c;
        if((c == '>' || c == '<') && (s._buffer[s._pos+1] == '='))
        {
            token += s._buffer[s._pos+1];
            s._pos++;
        }
        t = Token(token, -1);
        s._pos++;
    }

    return s;
}

bool STokenizer::get_token(int start_state, string &token)
{
    char c = _buffer[_pos];
    int state = _table[start_state][(int)c];
    string holding_string;
    if(!is_success(_table, state))
    {
        token += c;
    }
    while(state != -1 && more())
    {
        holding_string += c;
        if(is_success(_table, state))
        {
            token = holding_string;
        }

        _pos++;
        c = _buffer[_pos];
        state = _table[state][(int)c];
    }
    if(!is_success(_table, state))
    {
        return false;
    }
    return true;
}

void STokenizer::set_string(char str[])
{
    strcpy(_buffer, str);
}

void STokenizer::make_table(int _table[][MAX_COLUMNS])
{
    init_table(_table);
    //doubles:
    mark_fail(_table, 0);            //Mark states 0 and 2 as fail states
    mark_success(_table, 1);         //Mark states 1 and 3 as success states
    mark_fail(_table, 2);
    mark_success(_table, 3);

    mark_cells(0, _table, DIGITS, 1);    //state [0] --- DIGITS ---> [1]
    mark_cells(0, _table, '.', '.', 2);  //state [0] --- '.' ------> [2]
    mark_cells(1, _table, DIGITS, 1);    //state [1] --- DIGITS ---> [1]
    mark_cells(1, _table, '.', '.', 2);  //state [1] --- '.' ------> [2]
    mark_cells(2, _table, DIGITS, 3);    //state [2] --- DIGITS ---> [3]
    mark_cells(3, _table, DIGITS, 3);    //state [3] --- DIGITS ---> [3]

    //alphas:
    mark_fail(_table, 4);
    mark_success(_table, 5);

    mark_cells(4, _table, ALPHA, 5);
    mark_cells(5, _table, ALPHA, 5);

    //spaces:
    mark_success(_table, 6);
    mark_cells(6, _table, ' ', ' ',  6);

    //asterick:
    mark_success(_table, 11);
    mark_cells(11, _table, '*', '*', 11);

    //comma:
    mark_success(_table, 12);
    mark_cells(12, _table, ',', ',', 12);
}
