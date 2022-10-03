#include <cstdio>
#include <fstream>
#include <iostream>
#include <vector>
 
#include <gflags/gflags.h>

#include "inc/utils.h"
#include "inc/Log.h"
#include "inc/token.h"
#include "inc/PDouble.h"
#include "inc/config.h"

DEFINE_string(output_path,"","output file path");
DEFINE_string(input_path,"../ioFile/input.txt","input file path");
DEFINE_int32(log_state, 0, "log state level");

// TODO: need consider the error.
int main(int argc, char* argv[]) {
    gflags::ParseCommandLineFlags(&argc, &argv, true);
    if (RealPath(FLAGS_input_path).empty()) {
        std::cout << "Invalid input file." << std::endl;
        return 1;
    }
    if (!RealPath(FLAGS_output_path).empty()) freopen(FLAGS_output_path.data(), "w", stdout);
    Log::logState = FLAGS_log_state;
    Token::sizeVariable = 0, Token::sizeConst = 0, Token::sizeToken = 0;
    std::ifstream in(FLAGS_input_path);
    std::string contents((std::istreambuf_iterator<char>(in)), std::istreambuf_iterator<char>());
    int lpoint = 0, rpoint = (int)contents.size();
    int lineNumberContent = 1;
    std::vector<Token> tokens;
    std::vector<PDouble> consts;
    std::vector<std::string> variables;
    while (lpoint < rpoint) {
        // Process one sentence at a time.
        std::string sentence;
        while (IsBlank(contents[lpoint], lineNumberContent) && lpoint < rpoint) lpoint++;
        while (!IsBlank(contents[lpoint], lineNumberContent) && lpoint < rpoint) sentence += contents[lpoint++];
        if (sentence.empty() || lpoint >= rpoint) break;
        sentence += '\0';
        // State Convert
        SplitWords(sentence, lineNumberContent, tokens, consts, variables);
    }
    // TODO line number is error.
    // Log::i("Line Number: " + std::to_string(lineNumberContent));
    std::cout << "Tokens: ********************************************" << std::endl;
    for (auto it : tokens) Log::i(ParseCode(it.code) + ": " + it.to_string(consts, variables));
    std::cout << "Keywords: ********************************************" << std::endl;
    for (auto it : KEYWORDS_TABLE) Log::i(it);
    std::cout << "Variabels: ********************************************" << std::endl;
    for (auto it : variables) Log::i(it);
    std::cout << "Consts: ********************************************" << std::endl;
    for (auto it : consts) Log::i(std::to_string(it.value));
    return 0;
}
