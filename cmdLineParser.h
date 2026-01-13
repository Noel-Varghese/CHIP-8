#ifndef __CMDLINEPARSER_H__
#define __CMDLINEPARSER_H__

#include<iostream>
#include "logger.h"
using namespace std;

class TCmdLineParser{
    shared_ptr<TLogger> m_logger;
    string m_romFileName;
    int m_logLevel;
    void printHelpMessage();
    void setRomFileName(string name);
    void setLogLevel(int Loglevel);
    public:
        TCmdLineParser();
        void parseCMDLine(int argc, char** argv);//argc is number of arguments passed and argv is the raw input passed
        bool isRomFileSet();
        string getRomFileName();
        bool isLogLevelSet();
        int getLogLevel();
};

#endif