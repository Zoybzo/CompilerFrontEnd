#pragma once
#ifndef SDTER_INC_QUAT_H_
#define SDTER_INC_QUAT_H_

#include <cstring>
#include <stack>
#include <vector>
#include <unordered_map>
#include <map>

#include "inc/config.h"

class Quat {
public:
    
    Quat();
    virtual ~Quat();
    Quat(std::string _op, std::string _e1, std::string _e2, std::string e3);

    std::string op;
    std::string e1;
    std::string e2;
    std::string e3;

    std::string to_string();

private:
};

#endif //SDTER_INC_QUAT_H_ 