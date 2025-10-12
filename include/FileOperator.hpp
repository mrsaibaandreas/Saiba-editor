#pragma once
#include <string>


class FileOperator {
public:
    FileOperator();
    ~FileOperator();
    
    bool init();

    std::string getLine();

private:
    std::string fileName;

    bool openFile();
    bool closeFile();
};
