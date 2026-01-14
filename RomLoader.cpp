#include "romLoader.h"
using namespace std;

TRomLoader::TRomLoader(){
    m_logger = TLogger::getInstance();
}
TRomLoader::~TRomLoader(){

}

void TRomLoader::LoadRom(string &file_name, uint8_t* mem){
    m_logger->log("Loading Rom: "+file_name, ELogLevel::INFO);
    ifstream file(file_name, ios::binary);//opens the file in binary

    if(!file){
        m_logger->log("File not found: "+file_name, ELogLevel::ERROR);
        exit(1);
    }//checks if the file exists or not
    file.seekg(0, ios::end);//goes to the end of the file inorder to get the size of the file
    streamsize size = file.tellg();
    file.seekg(0, ios::beg); 
    if(file.read(reinterpret_cast<char*>(mem), size)){
        m_logger->log("File Read successfully, SIZE: "+ to_string(size)+"bytes. ", ELogLevel::INFO );
    }else{
        m_logger->log("ERROR DURING FILE READ: ", ELogLevel::ERROR);
    }
    file.close();
}
