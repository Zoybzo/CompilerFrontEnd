#pragma once
#ifndef SCANNER_INC_SCANNER_H_
#define SCANNER_INC_SCANNER_H_

#include <cstring>
#include <vector>
#include <unordered_map>
#include <set>

#include "inc/PDouble.h"
#include "inc/Token.h"

class Scanner {

public:
    Scanner();
    virtual ~Scanner();

    Scanner(std::vector<Token> _tokens, std::vector<PDouble> _consts, std::vector<std::string> _variabels);

    std::vector<Token> tokens;
    std::vector<PDouble> consts;
    std::vector<std::string> variables;
    
    void process(std::string contents, std::string &res);

private:
    void SplitWords(const std::string &str, int &lineNumberContent, std::vector<Token> &tokens, 
        std::vector<PDouble> &consts, std::vector<std::string> &variables, std::string &res);
    int JudgeDelimiter(const std::string &str, int &lpoint, std::string &out);
    void ProcessNumber(const std::string word, Token &token, std::vector<PDouble> &consts, std::string &res);
    void ProcessStr(const std::string word, Token &token,  std::vector<std::string> &variables, std::string &res);
    void ProcessDelimiter(const std::string word, int delimiterId, Token &token, std::string &res);
    int GetState(const std::unordered_map<bool(*)(const char &ch), int> &stateConvert, const char &ch);
    void AddToken(std::vector<Token> &tokens, Token token);
};
#endif //SCANNER_INC_SCANNER_H_