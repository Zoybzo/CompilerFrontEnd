#include "inc/config.h"

#include <string>

#include "inc/Log.h"

bool IsDigit(const char &ch) {
    return isdigit(ch);
}

bool IsChar(const char &ch) {
    return isalpha(ch) != 0;
}

bool IsEe(const char &ch) {
    return ch == 'E' || ch == 'e';
}

bool IsAddMinus(const char &ch) {
    return ch == '+' || ch == '-';
}

bool IsPoint(const char &ch) {
    return ch == '.';
}

bool IsDelimiter(const char &ch) {
    for (int i = 0; i < DELIMITER_NUMBER; ++i) {
        if (ch == DELIMITER_TABLE[i][0]) return true;
    }
    return false;
}

std::string ParseCode(int &code) {
    std::string str;
    switch (code) {
    case CONST_CODE:
        str = "double";
        break;
    case VAR_CODE:
        str = "variable";
        break;
    case DELIMITER_CODE:
        str = "delimiter";
        break;
    case KEYWORD_CODE:
        str = "keyword";
        break;
    // TODO need an error report
    default:
        break;
    }
    return str;
}