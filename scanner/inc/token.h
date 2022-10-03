#pragma once
#ifndef SCANNER_INC_TOKEN_H_
#define SCANNER_INC_TOKEN_H_

#include <string>
#include <vector>

#include "inc/PDouble.h"

class Token {
public:
    static int sizeToken;
    static int sizeConst;
    static int sizeVariable;

    Token();
    Token(int c, int v);
    virtual ~Token();

    int code; // token's type.
    int value;

    std::string to_string(std::vector<PDouble> &consts, std::vector<std::string> &variables);

private:
};

#endif // SCANNER_INC_TOKEN_H_