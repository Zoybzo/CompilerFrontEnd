#include "inc/utils.h"

#include <limits.h>
#include <sys/stat.h>
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