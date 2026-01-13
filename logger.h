#ifndef __LOGGER_H__
//if not defined is a preprocessor directive used for conditional compilation.
//It checks whether a specified macro has not been defined earlier in the file or in an included file
#define __LOGGER_H__
#include <iostream>
#include <fstream>
#include <memory>
using namespace std;

enum class ELogLevel{
    NONE = 0,
    ERROR = 1,
    WARN = 2,
    INFO = 3,
    DEBUG = 4
};

class TLogger{
    ELogLevel m_LogLevel;
    static shared_ptr<TLogger> m_LoggerInstance;//manager of the logger system 
    public:
    TLogger();
    void log(string msg, ELogLevel logLevel);
    void setLogLevel(ELogLevel logLevel);
    static shared_ptr<TLogger> getInstance();
};

#endif
//endif is for the ifndef 