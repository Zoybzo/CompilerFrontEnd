#include "inc/Quat.h"

Quat::Quat() {}

Quat::~Quat() {}

Quat::Quat(std::string _op, std::string _e1, std::string _e2, std::string _e3) {
    op = _op, e1 = _e1, e2 = _e2, e3 = _e3;
}

std::string Quat::to_string() {
    return "(" + op + "," + e1 + "," + e2 + "," + e3 + ")";
}