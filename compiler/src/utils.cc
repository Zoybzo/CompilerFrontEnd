#include "inc/utils.h"

#include <limits.h>
#include <sys/stat.h>

#include <algorithm>
#include <fstream>
#include <iostream>
#include <cmath>
#include <string>
#include <vector>

#include "inc/token.h"
#include "inc/config.h"
#include "inc/PDouble.h"
#include "inc/Log.h"

std::string RealPath(std::string path) {
    char realPathMem[PATH_MAX] = {0};
    char *realPathRet = nullptr;
    realPathRet = realpath(path.data(), realPathMem);
    if (realPathRet == nullptr) {
        std::cout << "File: " << path << " is not exist.";
        return "";
    }
    std::string realPath(realPathMem);
    std::cout << path << " realpath is: " << realPath << std::endl;
    return realPath;
}

bool IsBlank(const char &ch, int &lineNumberContent) {
    // New line is Special.
    if (ch == '\n') {
        lineNumberContent += 1;
        return true;
    }
    for (int i = 0; i < BLANK_NUMBER; ++i) if (ch == BLANK_TABLE[i]) return true;
    return false;
}

int GetState(const std::unordered_map<bool(*)(const char &ch), int> &stateConvert, const char &ch1) {
    for (auto it : stateConvert) { if (it.first(ch1)) return it.second;}
    return stateConvert.size();
}

// TODO need to modify if there is a delimiter consists of three of more chars
int JudgeDelimiter(const std::string &str, int &lpoint, std::string &out) {
    for (int i = 0; i < DELIMITER_NUMBER; i++) {
        if (str[lpoint] == DELIMITER_TABLE[i][0]) { // first check first char
            if ((int)DELIMITER_TABLE[i].size() == 2 && (int)str.size() >= 2 && str[lpoint + 1] == '=') {
                lpoint += 2;out = DELIMITER_TABLE[i];return i;
            } else if ((int)DELIMITER_TABLE[i].size() == 1) {
                lpoint += 1;out = DELIMITER_TABLE[i];return i;
            }
        }
    }
    return -1;
}

void SplitWords(const std::string &str, int &lineNumberContent, std::vector<Token> &tokens, std::vector<PDouble> &consts, std::vector<std::string> &variables, std::string &res) {
    int currentState = 0;
    int lpoint = 0, rpoint = (int)str.size();
    while (lpoint < rpoint) {
        std::string word, deWord;
        while (lpoint < rpoint) {
            Log::d("Before convert, currentState: " + std::to_string(currentState) + ", lpoint is " + std::to_string(lpoint) + ", current char is " + str[lpoint]);
            if (IsBlank(str[lpoint], lineNumberContent)) lpoint += 1;
            int delimiterId = -1;
            if (currentState == 0 && (delimiterId = JudgeDelimiter(str, lpoint, word)) != -1) {
                currentState = STATE_TABLE[currentState][DELIMITER_ID] - 1;
            } else {
                int check = GetState(stateConvert, str[lpoint]);
                currentState = STATE_TABLE[currentState][check] - 1;
                Log::d("Convert state: " + std::to_string(check));
                if (!IsBlank(str[lpoint], lineNumberContent) && currentState < 10 && currentState >= 0) word += str[lpoint++];
            }
            Log::d("After convert, currentState: " + std::to_string(currentState) + ", word is " + word);
            if (currentState == -1) Error(lineNumberContent);
            else if (currentState >= 10) {
                Token token;
                switch(currentState) {
                    case 10: {ProcessNumber(word, token, consts, res); break;}
                    case 11: {ProcessStr(word, token, variables, res); break;}
                    case 12: {}
                    case 13: {ProcessDelimiter(word, delimiterId, token, res); break;}
                    case 14: {}
                    default: {Error(lineNumberContent); break;}
                }
                AddToken(tokens, token);
                if (str[lpoint] == '\0') lpoint += 1;
                Log::d("Final word: " + word);
                Log::d("Final word' size is " + std::to_string(word.size()));
                word.clear(), currentState = 0;
            }
        }
    }
}

void ProcessNumber(const std::string word, Token &token, std::vector<PDouble> &consts, std::string &res) {
    Log::d("ProcessNumber Begin");
    double baseNumber = GetNumber(word);
    if (std::find(consts.begin(), consts.end(), baseNumber) == consts.end()) 
        token.code = CONST_CODE, token.value = Token::sizeConst++, consts.push_back(PDouble(baseNumber));
    getRes(res, token.code, token.value);
    Log::d("ProcessNumber End");
}

void ProcessStr(const std::string word, Token &token, std::vector<std::string> &variables, std::string &res) {
    Log::d("Give me the word: "  + word);
    auto it = std::find(KEYWORDS_TABLE.begin(), KEYWORDS_TABLE.end(), word);
    int val = -1, code = VAR_CODE;
    if (it != KEYWORDS_TABLE.end()) {
        token.code=KEYWORD_CODE;
        token.value = it-KEYWORDS_TABLE.begin();
        val = token.value, code = KEYWORD_CODE;
    } else {
        auto tar = std::find(variables.begin(), variables.end(), word);
        val = tar - variables.begin();
        if (tar == variables.end()) {
            token.code = VAR_CODE;
            token.value = Token::sizeVariable++;
            variables.push_back(word);
            val = token.value;
        } else {
            token.code = VAR_CODE, token.value = val;
        }
    }
    getRes(res, code, val);
}

void ProcessDelimiter(const std::string word, int delimiterId, Token &token, std::string &res) {
    if (delimiterId == -1) {for (int i = 0; i < DELIMITER_NUMBER; i++) if (DELIMITER_TABLE[i] == word) {delimiterId = i; break;}}
    token.code = DELIMITER_CODE, token.value = delimiterId;
    getRes(res, token.code, token.value);
}

void AddToken(std::vector<Token> &tokens, Token token) {
    if (token.code == -1) return ;
    tokens.push_back(token);
    Token::sizeToken += 1;
}

double GetNumber(const std::string &str) {
    Log::d("GetNumber: " + str);
    int lpoint = 0, rpoint = (int)str.size();
    bool dec = false, exp = false, negExp = false;
    double baseNumber=0, expNumber = 0, decNumber = 1;
    while (lpoint < rpoint) {
        if (isdigit(str[lpoint])) {
            if (exp) (expNumber *= 10) += str[lpoint] - '0';
            else if (dec) decNumber *= 0.1, baseNumber += decNumber * (str[lpoint]-'0');
            else (baseNumber*=10)+=(str[lpoint]-'0');
        } 
        else if (str[lpoint] == 'e' || str[lpoint] == 'E') exp = true;
        else if (str[lpoint] == '.') dec = true;
        lpoint += 1;
    }
    if (exp) baseNumber *= pow(10, negExp ? -expNumber : expNumber);
    return baseNumber;
}

void Error(const int &lineNumberContent) {
    Log::e("Error in: " + std::to_string(lineNumberContent));
    exit(1);
}

int Sgn(double x) {
    if (fabs(x) < EPS) return 0;
    if (x < 0) return -1;
    else return 1;
}

void getRes(std::string &res, int code, int val) {
    res = res + "<" + ParseCode(code) + "," + std::to_string(val) + ">;";
}