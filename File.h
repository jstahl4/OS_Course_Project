/*
Tom Kueny
	+ James Stahl
*/
#include <iostream>
class File{
public:
    File(std::string fileName);
    File(std::string fileName, int blockSize, char* buffer = NULL);

    // accessors
    string get_name();
    int get_size();
    char * get_data();

    // mutators
    void set_data(char*);
    

private:
    static int block_size;
    char* data;
    std::string name;
    int size;
};
