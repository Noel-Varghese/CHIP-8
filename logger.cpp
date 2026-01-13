#include <iostream>
#include <memory>
#include <string>
#include "logger.h"

using namespace std;

shared_ptr<TLogger> TLogger::m_LoggerInstance = nullptr;

TLogger::TLogger():m_LogLevel(ELogLevel::DEBUG){//constructor

}

void TLogger::log(string msg, ELogLevel logLevel){
    string buff = "";
    if(logLevel<=m_LogLevel){
        switch(logLevel){
            case ELogLevel::ERROR:
                buff+="\033[31m[Error]";
                break;
            case ELogLevel::WARN:
                buff+="\033[33m[Warn]";
                break;
            case ELogLevel::INFO:
                buff += "\033[32m[INFO]";
                break;
            case ELogLevel::DEBUG://usually ignored due to its messy nature
                buff += "\033[0m[Debug]";
                break;
            default:break;
        }
        buff += msg + "\n";
        cout<<buff;
    }
}

void TLogger::setLogLevel(ELogLevel LogLevel){
    m_LogLevel = LogLevel;
}

shared_ptr<TLogger> TLogger::getInstance(){
    if(m_LoggerInstance == nullptr){
        m_LoggerInstance = shared_ptr<TLogger> (new(TLogger));
    
    }
    return m_LoggerInstance;
}
