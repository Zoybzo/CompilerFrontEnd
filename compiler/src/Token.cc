#include "inc/Token.h"

#include <iostream>
#include <string>
#include <vector>

#include "inc/config.h"

Token::Token() {}

Token::Token(int c, int v) {
    code = c;
    value = v;
}

Token::~Token() {}

int Token::sizeVariable = 0;
int Token::sizeToken = 0;
int Token::sizeConst = 0;

std::string Token::to_string(std::vector<PDouble> &consts, std::vector<std::string> &variables) {
    std::string ret = "";
    if (code == VAR_CODE) ret = variables[value];
    else if (code == CONST_CODE) ret = (std::to_string(consts[value].value));
    else if (code == DELIMITER_CODE) ret = (DELIMITER_TABLE[value]);
    else if (code == KEYWORD_CODE) ret = KEYWORDS_TABLE[value];
    else if (code == END_CODE) ret = "#";
    return ret;
}