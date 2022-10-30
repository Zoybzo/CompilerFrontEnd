#include "inc/Log.h"

#include <iostream>

Log::Log() {}

Log::~Log() {}

void Log::i(const std::string &str) {
    if (Log::logState >= LOG_INFO) std::cout << "Info: [" << str << "]" << std::endl;
}

void Log::e(const std::string &str) {
    if (Log::logState >= LOG_ERROR) std::cout << "Error: [" << str << "]" << std::endl;
}

void Log::d(const std::string &str) {
    if (Log::logState >= LOG_DEBUG) std::cout << "Debug: [" << str << "]" << std::endl;
}

int Log::logState;