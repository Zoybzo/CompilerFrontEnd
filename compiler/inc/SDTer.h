#pragma once
#ifndef SDTER_INC_SDTER_H_
#define SDTER_INC_SDTER_H_

#include <cstring>
#include <stack>
#include <vector>
#include <unordered_map>
#include <map>

#include "inc/Token.h"
#include "inc/StackElement.h"
#include "inc/config.h"
#include "inc/Quat.h"

#define NON_TERMINAL_NUMBER 5
#define TERMINAL_NUMBER 8

class SDTer {
public:
    
    SDTer();
    virtual ~SDTer();
    SDTer(std::vector<Token> _tokens, std::vector<PDouble> _consts, std::vector<std::string> _variables);

    std::vector<Token> tokens;
    std::vector<PDouble> consts;
    std::vector<std::string> variables;

    std::stack<StackElement> sem;
    std::stack<StackElement> syn;

    std::vector<Quat> quat;

    void init();
    void process();

private:
    std::unordered_map<std::string, std::vector<int> > llsyn;
    std::unordered_map<std::string, int> NON_TERMINAL_TABLE;
    std::unordered_map<std::string, int> TERMINAL_TABLE;
    int SDTER_STATE_TABLE[NON_TERMINAL_NUMBER][TERMINAL_NUMBER];
    std::vector<std::vector<StackElement> > SDTER_RIGHT;
};

#endif //SDTER_INC_SDTER_H_ 