#include "inc/SDTer.h"

#include <string.h>

#include <algorithm>
#include <vector>
#include <cstring>

#include "inc/Token.h"
#include "inc/config.h"
#include "inc/Log.h"
#include "inc/StackElement.h"
#include "inc/Quat.h"

SDTer::SDTer() {
    init();
}

SDTer::~SDTer() {}

SDTer::SDTer(std::vector<Token> _tokens, std::vector<PDouble> _consts, std::vector<std::string> _variables) {
    init();
    tokens = _tokens;
    consts = _consts;
    variables = _variables;
}

void SDTer::init() {
    NON_TERMINAL_TABLE = {{"E", 0}, {"E'", 1}, {"T", 2}, {"T'", 3}, {"F", 4}};
    llsyn = {
        {"E", {1}}, {"E'", {2, 3, 4}}, {"T", {5}}, {"T'", {6, 7, 8}}, {"F", {9, 10}}
    };
    TERMINAL_TABLE = {{"i", 0}, {"+", 1}, {"-", 2}, {"*", 3}, {"/", 4}, {"(", 5}, {")", 6}, {"#", 7}};
    int TMP_SDTER_STATE_TABLE[NON_TERMINAL_NUMBER][TERMINAL_NUMBER] = {
        1, -1, -1, -1, -1, 1, -1, -1,
        -1, 2, 3, -1, -1, -1, 4, 4,
        5, -1, -1, -1, -1, 5, -1, -1,
        -1, 8, 8, 6, 7, -1, 8, 8,
        9, -1, -1, -1, -1, 10, -1, -1
    };
    for (int i = 0; i < NON_TERMINAL_NUMBER; ++i) for (int j = 0; j < TERMINAL_NUMBER; ++j) 
        SDTER_STATE_TABLE[i][j] = TMP_SDTER_STATE_TABLE[i][j];
    SDTER_RIGHT = {
        {StackElement(STK_NT, "E'"), StackElement(STK_NT, "T"),},
        {StackElement(STK_NT, "E'"),StackElement(STK_OP, "GEQ+"),
        StackElement(STK_NT, "T"),  StackElement(STK_ELE, "+"),},
        {StackElement(STK_NT, "E'"),StackElement(STK_OP, "GEQ-"),
        StackElement(STK_NT, "T"), StackElement(STK_ELE, "-"),},
        {}, // epsilon
        {StackElement(STK_NT, "T'"),StackElement(STK_NT, "F"),},
        {StackElement(STK_NT, "T'"),StackElement(STK_OP, "GEQ*"),
        StackElement(STK_NT, "F"),StackElement(STK_ELE, "*"),},
        {StackElement(STK_NT, "T'"),StackElement(STK_OP, "GEQ/"),
        StackElement(STK_NT, "F"),StackElement(STK_ELE, "/"),},
        {}, //epsilon
        {StackElement(STK_OP, "PUSHi"),StackElement(STK_ELE, "i"),},
        {StackElement(STK_ELE, ")"),StackElement(STK_NT, "E"),
        StackElement(STK_ELE, "("), }
    };
}

void SDTer::process() {
    Log::d("[D]Process begin");
    syn.push(StackElement(STK_ELE, "#"));
    syn.push(StackElement(STK_NT, "E"));
    int l = 0, r = (int)tokens.size();
    if (tokens[r-1].code != END_CODE) tokens.push_back(Token(END_CODE, 0)), r += 1;
    while(!syn.empty()) {
        StackElement now = syn.top();
        syn.pop();
        if (now.value == "") continue;
        Log::d("[D]syn_now: " + now.value);
        switch (now.type) {
            case STK_OP: {
                std::string de;
                de+=now.value[(int)now.value.size()-1];
                if (de == "+" || de == "-" || de == "*" || de == "/") {
                    std::string e2 = sem.top().value;
                    sem.pop();
                    std::string e1 = sem.top().value;
                    sem.pop();
                    std::string e3 = "t" + std::to_string((int)quat.size());
                    quat.push_back(Quat(de, e1, e2, e3));
                    Log::d("[D]quat: " + Quat(de, e1, e2, e3).to_string());
                    sem.push(StackElement(STK_ELE, e3));
                } else {
                    sem.push(StackElement(STK_ELE, now.value.substr(4)));
                }
                break;
            }
            case STK_ELE: {
                if (now.value == tokens[l].to_string(consts, variables)) {
                    Log::d("[D]match: " + now.value + ", " + tokens[l].to_string(consts, variables));
                    l += 1;
                    continue;
                }
                break;
            }
            case STK_NT: {
                std::string token = tokens[l].to_string(consts, variables);
                Log::d("[D]token: " + token);
                int nxt = SDTER_STATE_TABLE[NON_TERMINAL_TABLE[now.value]][TERMINAL_TABLE[token]] - 1;
                Log::d("[D]nxt: " + std::to_string(nxt));
                if (nxt != 8) {for (auto it : SDTER_RIGHT[nxt]) syn.push(it);}
                else {syn.push(StackElement(STK_OP, "PUSH"+token));syn.push(StackElement(STK_ELE, token));}
                break;
            }
            default: {
                break;
            }
        }
    }
    Log::d("[D]Process End");
}