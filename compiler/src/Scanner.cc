#include "inc/Scanner.h"

#include <limits.h>
#include <algorithm>
#include <fstream>
#include <iostream>
#include <cmath>
#include <string>
#include <vector>

#include "inc/Token.h"
#include "inc/config.h"
#include "inc/PDouble.h"
#include "inc/Log.h"
#include "inc/utils.h"

Scanner::Scanner() {}
Scanner::~Scanner() {}

Scanner::Scanner(std::vector<Token> _tokens, std::vector<PDouble> _consts, std::vector<std::string> _variabels) {
    tokens = _tokens;
    consts = _consts;
    variables = _variabels;
}

void Scanner::process(std::string contents, std::string &res) {
    int lpoint = 0, rpoint = (int)contents.size();
    int lineNumberContent = 1;
    while (lpoint < rpoint) {
        // Process one sentence at a time.
        std::string sentence;
        while (IsBlank(contents[lpoint], lineNumberContent) && lpoint < rpoint) lpoint++;
        while (!IsBlank(contents[lpoint], lineNumberContent) && lpoint < rpoint) sentence += contents[lpoint++];
        std::cout << sentence << std::endl;
        if (sentence.empty() || lpoint > rpoint) break;
        sentence += '\0';
        // State Convert
        SplitWords(sentence, lineNumberContent, tokens, consts, variables, res);
    }
}

void Scanner::SplitWords(const std::string &str, int &lineNumberContent, std::vector<Token> &tokens, std::vector<PDouble> &consts, std::vector<std::string> &variables, std::string &res) {
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

int Scanner::JudgeDelimiter(const std::string &str, int &lpoint, std::string &out) {
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

void Scanner::ProcessNumber(const std::string word, Token &token, std::vector<PDouble> &consts, std::string &res) {
    Log::d("ProcessNumber Begin");
    double baseNumber = GetNumber(word);
    if (std::find(consts.begin(), consts.end(), baseNumber) == consts.end()) 
        token.code = CONST_CODE, token.value = Token::sizeConst++, consts.push_back(PDouble(baseNumber));
    getRes(res, token.code, token.value);
    Log::d("ProcessNumber End");
}

void Scanner::ProcessStr(const std::string word, Token &token, std::vector<std::string> &variables, std::string &res) {
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

void Scanner::ProcessDelimiter(const std::string word, int delimiterId, Token &token, std::string &res) {
    if (delimiterId == -1) {for (int i = 0; i < DELIMITER_NUMBER; i++) if (DELIMITER_TABLE[i] == word) {delimiterId = i; break;}}
    token.code = DELIMITER_CODE, token.value = delimiterId;
    getRes(res, token.code, token.value);
}

int Scanner::GetState(const std::unordered_map<bool(*)(const char &ch), int> &stateConvert, const char &ch1) {
    for (auto it : stateConvert) { if (it.first(ch1)) return it.second;}
    return stateConvert.size();
}


void Scanner::AddToken(std::vector<Token> &tokens, Token token) {
    if (token.code == -1) return ;
    tokens.push_back(token);
    Token::sizeToken += 1;
}