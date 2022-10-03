#ifndef SCANNER_INC_LOG_H_
#define SCANNER_INC_LOG_H_

#include <string>

#define LOG_INFO 0
#define LOG_DEBUG 1
#define LOG_ERROR 2

class Log {
public:
    Log();
    virtual ~Log();

    static int logState;

    static void i(const std::string &str);
    static void d(const std::string &str);
    static void e(const std::string &str);
};

#endif //SCANNER_INC_LOG_H_