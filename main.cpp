#include <iostream>
#include "logger.h"
#include "cmdLineParser.h"
using namespace std;

int main(int argc, char** argv){//helps in taking in the roms later on :)
    TCmdLineParser cmdParser;
    shared_ptr<TLogger> logger = TLogger::getInstance();
    cmdParser.parseCMDLine(argc, argv);
    if(cmdParser.isLogLevelSet()){
        //shared_ptr<TLogger> logger = TLogger::getInstance();
        switch (cmdParser.getLogLevel())
        {
            case 0: logger->setLogLevel(ELogLevel::NONE); break;
            case 1: logger->setLogLevel(ELogLevel::ERROR); break;
            case 2: logger->setLogLevel(ELogLevel::WARN); break;
            case 3: logger->setLogLevel(ELogLevel::INFO); break;
            case 4: logger->setLogLevel(ELogLevel::DEBUG); break;
        }
    }
    if(!cmdParser.isRomFileSet()){
        logger->log("No Rom Path provided", ELogLevel::ERROR);
        exit(1);
    }
    logger->log("Rom path provided: "+cmdParser.getRomFileName(), ELogLevel::INFO);
    return 0;
}