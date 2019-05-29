//
// Created by lalof on 5/21/2019.
//

#include "token.h"

using namespace std;

Token::Token()
{
    _token = "";
    _type = -1;
}

Token::Token(std::string str, int type)
{
    _token = str;
    _type = type;
}

ostream& operator <<(ostream& os, const Token& t)
{
    os << "|" << t.token_str() << "|";
    return os;
}

int Token::type() const
{
    return this->_type;
}

string Token::type_string() const
{
    const int what_is_this = this->type();
    switch(what_is_this)
    {
        case 1:
            return "ALHPA";
            break;
        case 2:
            return "NUMBER";
            break;
        case 3:
            return "SPACE";
        case 4:
            return "ASTERISK";
        case 5:
            return "COMMA";
        default:
            return "UNKNOWN";
            break;
    }
    return "ERROR";
}

string Token::token_str() const
{
    return this->_token;
}
