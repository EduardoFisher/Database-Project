//
// Created by lalof on 5/21/2019.
//

#ifndef MYDB_TOKEN_H
#define MYDB_TOKEN_H

#include <iostream>

class Token
{
public:
    Token();
    Token(std::string str, int type);
    friend std::ostream& operator <<(std::ostream& os, const Token& t);
    int type() const;
    std::string type_string() const;
    std::string token_str() const;
private:
    std::string _token;
    int _type;
};


#endif //MYDB_TOKEN_H
