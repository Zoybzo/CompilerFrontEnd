#pragma once
#ifndef SCANNER_INC_UTILS_H_
#define SCANNER_INC_UTILS_H_

#include <unordered_map>
#include <set>
#include <string>
#include <vector>

#include "inc/token.h"
#include "inc/PDouble.h"

#define EPS 1e-8

std::string RealPath(std::string path);
bool IsBlank(const char &ch, int &lineNumberContent);
void Error(const int &lineNumberContent);
void SplitWords(const std::string &str, int &lineNumberContent, std::vector<Token> &tokens, std::vector<PDouble> &consts, std::vector<std::string> &variables);
double GetNumber(const std::string &str);
int GetState(const std::unordered_map<bool(*)(const char &ch), int> &stateConvert, const char &ch);
int JudgeDelimiter(const std::string &str, int &lpoint, std::string &out);
void ProcessNumber(const std::string word, Token &token, std::vector<PDouble> &consts);
void ProcessStr(const std::string word, Token &token,  std::vector<std::string> &variables);
void ProcessDelimiter(const std::string word, int delimiterId, Token &token);
void AddToken(std::vector<Token> &tokens, Token token);

int Sgn(double x);
#endif //SCANNER_INC_UTILS_H_