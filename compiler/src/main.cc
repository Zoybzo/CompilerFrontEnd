#include <cstdio>
#include <fstream>
#include <iostream>
#include <vector>
 
#include <gflags/gflags.h>

#include "inc/utils.h"
#include "inc/Log.h"
#include "inc/Token.h"
#include "inc/PDouble.h"
#include "inc/config.h"
#include "inc/Scanner.h"
#include "inc/SDTer.h"
#include "inc/Quat.h"

DEFINE_string(output_path,"../ioFile/output.txt","output file path");
DEFINE_string(input_path,"../ioFile/input.txt","input file path");
DEFINE_int32(log_state, 0, "log state level");

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
    
    std::string res;
    Scanner scanner;
    scanner.process(contents, res);

    std::cout << "Res: " << res << std::endl;

    std::vector<Token> tokens = scanner.tokens;
    std::vector<PDouble> consts = scanner.consts;
    std::vector<std::string> variables = scanner.variables;

    SDTer sdter(tokens, consts, variables);
    sdter.process();

    std::vector<Quat> quat = sdter.quat;
    for (auto it : quat) Log::i(it.to_string());
    
    return 0;
}
