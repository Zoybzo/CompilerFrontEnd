#pragma once
#ifndef SCANNER_INC_CONFIG_H_
#define SCANNER_INC_CONFIG_H_

#include <unordered_map>
#include <string>
#include <vector>

// TODO what is this?
#define VDIGIT 0
#define VPOINT 1
#define VEXPONENT 2
#define VADDMINUS 3
#define VCHAR 4
#define VDELIMITER 5
#define VELSE 6

// to define tables.
#define STATE_NUMBER 10
#define CHAR_NUMBER 7
#define KEYWORDS_NUMBER 13
#define DELIMITER_NUMBER 15
#define BLANK_NUMBER 6

// to convert state
#define DIGIT_ID 0
#define POINT_ID 1
#define EE_ID 2
#define AM_ID 3
#define CHAR_ID 4
#define DELIMITER_ID 5

// code in token
#define DOUBLE_CODE 1
#define VAR_CODE 2
#define DELIMITER_CODE 3
#define KEYWORD_CODE 4

// TODO table has been changed.
// changed the delimiter.
int const STATE_TABLE[STATE_NUMBER][CHAR_NUMBER] =
   //d, ., E|e, +\-, ch, deli, else 
    {2, 15, 8, 15, 8, 14, 15, 
     2, 3, 5, 11, 0, 11, 11,
     4, 0, 0, 0, 0, 0, 0,
     4, 0, 5, 11, 0, 11, 11,
     7, 0, 0, 6, 0, 0, 0,
     7, 0, 0, 0, 0, 0, 0,
     7, 0, 0, 11, 0, 0, 11,
     8, 0, 8, 12, 8, 12, 12,
     14, 0, 14, 0, 14, 10, 14,
     13, 0, 13, 0, 13, 0, 13};

const std::vector<std::string> KEYWORDS_TABLE = {"program", "begin", "end", "var", "while", "do", "repeat", "until", "for", "to", "if", "then", "else"};
const std::vector<std::string> DELIMITER_TABLE = {":=", ">=", "<=", "==", ";", ":", "(", ")", ",", "+", "-", "*", "/", ">", "<"};
const std::vector<char> BLANK_TABLE = {'\n', '\t', '\r', '\b', ' ', '\0'};

bool IsDigit(const char &ch);
bool IsPoint(const char &ch);
bool IsEe(const char &ch);
bool IsAddMinus(const char &ch);
bool IsChar(const char &ch);
bool IsDelimiter(const char &ch);

const std::unordered_map<bool(*)(const char &ch), int> stateConvert = 
{{IsDelimiter, DELIMITER_ID}, {IsChar, CHAR_ID}, {IsAddMinus, AM_ID},{IsEe, EE_ID}, {IsPoint, POINT_ID}, {IsDigit, DIGIT_ID}};

std::string ParseCode(int &code);

#endif // SCANNER_INC_CONFIG_H_