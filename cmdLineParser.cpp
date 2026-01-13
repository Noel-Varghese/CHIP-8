#include <cctype>
#include "cmdLineParser.h"
using namespace std;

void TCmdLineParser::printHelpMessage(){
    cout << "Usage: lcc [options] file..." <<endl;
    cout << "Options:" <<endl;
    cout << "   -h, --help              Display this help information" << endl;
    cout << "   -r, --romFileName       Set the rom file path to be used" << endl;
    cout << "   -l, --logLevel          Set the desired log level [NONE = 0, ERROR = 1, WARN = 2, INFO = 3, DEBUG = 4]" <<endl;
}

void TCmdLineParser::setRomFileName(string new_name){
    this->m_romFileName = new_name;
}//this sets rom file name 

void TCmdLineParser::setLogLevel(int logLevel){
    this->m_logLevel = logLevel;
}//sets the level for the log system

TCmdLineParser::TCmdLineParser():m_romFileName(""), m_logLevel(-1){
    m_logger = TLogger::getInstance();
}

void TCmdLineParser::parseCMDLine(int argc, char** argv){
    for(int i=1;i<argc;i++){
        string auxStr(argv[i]);
        if(auxStr.at(0) == '-'){
            if(auxStr == "-h"||auxStr == "-help"){
                this->printHelpMessage();
            }
            else if(auxStr == "-r"||auxStr == "--romFileName"){
                i++;
                string auxName(argv[i]);
                this->setRomFileName(auxName);//takes in rom name
            }
            else if(auxStr == "-l" || auxStr == "--logLevel"){
                i++;
                string logLevel(argv[i]);
                if(!isdigit(logLevel.at(0))){
                    m_logger->log("Parameter must be between[0,4]", ELogLevel::ERROR);
                    exit(1);
                }//check if log level is valid
                this->setLogLevel(stoi(logLevel));//log is string by default so it changes it into an integer
            }
            else{
                string param(argv[i]);
                m_logger->log("unknown parameter: "+param, ELogLevel::ERROR);
                exit(1);
            }
        }
        else{
            string param(argv[i]);
            m_logger->log("unknown parameter: "+param, ELogLevel::ERROR);
            exit(1);
        }
    }
}

bool TCmdLineParser::isRomFileSet(){
    return !(m_romFileName == "");
}

string TCmdLineParser::getRomFileName(){
    return m_romFileName;
}

bool TCmdLineParser::isLogLevelSet(){
    return !(m_logLevel == -1);
}

int TCmdLineParser::getLogLevel(){
    return m_logLevel;
}
