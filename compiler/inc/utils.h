#pragma once
#ifndef SCANNER_INC_UTILS_H_
#define SCANNER_INC_UTILS_H_

#include <string>

#include "inc/Token.h"
#include "inc/PDouble.h"

#define EPS 1e-8

std::string RealPath(std::string path);
bool IsBlank(const char &ch, int &lineNumberContent);
void Error(const int &lineNumberContent);
double GetNumber(const std::string &str);
void getRes(std::string &res, int code, int val);
int Sgn(double x);

#endif //SCANNER_INC_UTILS_H_