/*
Tom Kueny
*/
#include <iostream>
class File{
public:
    File(std::string fileName);
    File(std::string fileName, int blockSize, char* buffer = NULL);
private:
    char* data;
    std::string name;
    int size;
};
